/*
 * This file is part of LunaDash (https://github.com/LisaRowell/LunaDash)
 * Copyright (C) 2023 Lisa Rowell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MQTTClient.h"

#include "XMLSourcedEntity.h"
#include "ClientStatusVariable.h"
#include "Variables.h"
#include "Variable.h"
#include "Topic.h"
#include "XMLFileReader.h"

#include <MQTTAsync.h>

#include <QVector>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QMessageBox>
#include <QObject>
#include <QStringView>

const QVector<QString> MQTTClient::allowedAttrs = { "server", "port" };
const QVector<QString> MQTTClient::requiredAttrs = { "server" };

MQTTClient::MQTTClient(XMLFileReader &xmlReader,  Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    connect(this, &MQTTClient::connectedSignal, this, &MQTTClient::connected);
    connect(this, &MQTTClient::connectionLostSignal,
            this, &MQTTClient::disconnected);
    connect(this, &MQTTClient::subscriptionSuccessSignal,
            this, &MQTTClient::handleSubscriptionSuccess);
    connect(this, &MQTTClient::subscriptionFailureSignal,
            this, &MQTTClient::handleSubscriptionFailure);
    connect(this, &MQTTClient::messageForUnknownTopicSignal,
            this, &MQTTClient::messageForUnknownTopic);

    parseAttributes(xmlReader);

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        QStringView elementName = xmlReader.name();
        if (elementName.compare("ClientId") == 0) {
            clientId.set(xmlReader, serverName);
        } else if (elementName.compare("StatusVariable") == 0) {
            addStatusVariable(xmlReader, variables);
        } else if (elementName.compare("Topic") == 0) {
            addTopic(xmlReader, variables);
        } else {
            unsupportedChildElement("MQTTBroker", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }

    disconnected();
    startConnection();
}

void MQTTClient::parseAttributes(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader);

    serverName = stringAttribute("server", xmlReader);
    port = ushortAttribute("port", xmlReader, 1883);
}

void MQTTClient::startConnection() {
    const QString serverURI = "tcp://" + serverName + ":"
                              + QString::number(port);
    const QByteArray localServerURI = serverURI.toLocal8Bit();

    int error;
    error = MQTTAsync_create(&handle, localServerURI.data(),
                             clientId.cString(), MQTTCLIENT_PERSISTENCE_NONE,
                             NULL);
    if (error != MQTTASYNC_SUCCESS) {
        mqttError("MQTT client create", error);
    }

    error = MQTTAsync_setCallbacks(handle, this,
                                   connectionLostCallback,
                                   messageArrivedCallback,
                                   deliveryCompleteCallback);
    if (error != MQTTASYNC_SUCCESS) {
        mqttError("MQTT set callbacks", error);
    }

    error = MQTTAsync_setConnected(handle, this, connectedCallback);
    if (error != MQTTASYNC_SUCCESS) {
        mqttError("MQTT set connected callback", error);
    }

    // Due to an incompatibility in the Paho library with C++, we can't use
    // the default initializer, zero the structure and set each currently
    // implement field. While the memset isn't theoretically needed, it's
    // an insurance policy against library changes that add options.
    MQTTAsync_connectOptions connectOptions;
    memset(&connectOptions, 0, sizeof(MQTTAsync_connectOptions));
    connectOptions.struct_id[0] = 'M';
    connectOptions.struct_id[1] = 'Q';
    connectOptions.struct_id[2] = 'T';
    connectOptions.struct_id[3] = 'C';
    connectOptions.struct_version = 6;
    connectOptions.keepAliveInterval = 60;
    connectOptions.cleansession = true;
    connectOptions.maxInflight = 65535;
    connectOptions.will = NULL;
    connectOptions.username = NULL;
    connectOptions.password = NULL;
    connectOptions.connectTimeout = 30;
    connectOptions.retryInterval = 0;
    connectOptions.ssl = NULL;
    connectOptions.onSuccess = NULL;
    connectOptions.onFailure = NULL;
    connectOptions.context = this;
    connectOptions.serverURIcount = 0;
    connectOptions.serverURIs = NULL;
    connectOptions.MQTTVersion = MQTTVERSION_DEFAULT;
    connectOptions.automaticReconnect = true;
    connectOptions.minRetryInterval = 1;
    connectOptions.maxRetryInterval = 60;
    connectOptions.binarypwd.len = 0;
    connectOptions.binarypwd.data = NULL;
    connectOptions.cleanstart = false;
    connectOptions.connectProperties = NULL;
    connectOptions.willProperties = NULL;
    connectOptions.onSuccess5 = NULL;
    connectOptions.onFailure5 = NULL;

    error = MQTTAsync_connect(handle, &connectOptions);
    if (error != MQTTASYNC_SUCCESS) {
        mqttError("MQTT connect", error);
    }
}

void MQTTClient::connectedCallback(void *context, char *cause) {
    (void)cause;

    MQTTClient *client = (MQTTClient *)context;
    client->connectedCallbackInvoked();
}

void MQTTClient::connectedCallbackInvoked() {
    emit connectedSignal();
}

void MQTTClient::connected() {
    for (auto statusVariable : statusVariables) {
        statusVariable->set(true);
    }

    subscribeToTopics();
}

void MQTTClient::connectionLostCallback(void *context, char *cause) {
    (void)cause;

    MQTTClient *client = (MQTTClient *)context;
    client->connectionLostCallbackInvoked();
}

void MQTTClient::connectionLostCallbackInvoked() {
    emit connectionLostSignal();
}

void MQTTClient::disconnected() {
    for (auto statusVariable : statusVariables) {
        statusVariable->set(false);
    }
}

// Static member function used as a callback from the Paho C MQTT library
int MQTTClient::messageArrivedCallback(void *context, char *topicName,
                                       int topicLen,
                                       MQTTAsync_message *message) {
    MQTTClient *client = (MQTTClient *)context;

    QString topicPath;
    if (topicLen == 0) {
        topicPath = topicName;
    } else {
        topicPath = QString::fromUtf8(topicName, topicLen);
    }

    QString payload = QString::fromUtf8((const char *)message->payload,
                                        message->payloadlen);

    client->messageArrivedCallbackInvoked(topicPath, payload);

    MQTTAsync_free(topicName);
    MQTTAsync_freeMessage(&message);

    return true;
}

// Class member function for the above callback.
void MQTTClient::messageArrivedCallbackInvoked(const QString &topicPath,
                                               const QString &payload) {
    // Though we are running on a MQTT library thread and not the UI thread,
    // it should be safe to access the topic map since all topic creation
    // is done before the broker connection is started.
    if (topics.contains(topicPath)) {
        Topic *topic = topics.value(topicPath);
        topic->messageReceived(payload);
    } else {
        emit messageForUnknownTopicSignal(topicPath);
    }
}

void MQTTClient::messageForUnknownTopic(const QString &topicPath) {
    QString warningStr;
    QTextStream warningStream(&warningStr);
    warningStream << "Received a MQTT publish message from '" << serverName
                  << "' for unknown topic '" << topicPath << "'." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Publish for Unknown Topic", warningStr);
}

void MQTTClient::deliveryCompleteCallback(void *context,
                                          MQTTAsync_token token) {
    (void)context;
    (void)token;
}

void MQTTClient::addStatusVariable(XMLFileReader &xmlReader,
                                   Variables &variables) {
    ClientStatusVariable *statusVariable =
        new ClientStatusVariable(xmlReader, variables);
    variables.addVariable(statusVariable);
    statusVariables.append(statusVariable);
}

void MQTTClient::addTopic(XMLFileReader &xmlReader, Variables &variables) {
    Topic *topic = new Topic(xmlReader, variables, this);
    topics.insert(topic->path(), topic);
}

void MQTTClient::subscribeToTopics() const {
    for (auto const &topic : topics) {
        subscribe(topic);
    }
}

void MQTTClient::subscribe(const Topic *topic) const {
    const QString &topicPath = topic->path();
    QByteArray topicPathByteArray = topicPath.toLocal8Bit();
    const char *topicPathCString = topicPathByteArray.data();

    MQTTAsync_responseOptions responseOptions =
        MQTTAsync_responseOptions_initializer;
    responseOptions.onSuccess = subscribeSuccessCallback;
    responseOptions.onFailure = subscribeFailureCallback;
    responseOptions.context = (void *)topic; // Evil, but valid cast

    int error = MQTTAsync_subscribe(handle, topicPathCString, 0, &responseOptions);
    if (error != MQTTASYNC_SUCCESS) {
        mqttError("Send Subscribe", error);
    }
}

// Static callback invoked by the C MQTT library
void MQTTClient::subscribeSuccessCallback(void *context,
                                          MQTTAsync_successData *response) {
    // Note that this callback is not running on the UI thread, but the method
    // call to topic should be okay since all topics are configured before the
    // connection to the broker is made and any subscriptions sent.
    const Topic *topic = (Topic *)context;
    const QString &topicPath = topic->path();

    MQTTClient *client = topic->mqttClient();
    client->subscribeSuccessCallbackInvoked(topicPath);

    // The Paho example documentation does not free the MQTTAsync_successData
    // structure so we're currently assuming that, unlike topics, it's freed
    // by the caller. It's work investing further though...
    (void)response;
}

// Class method for the above static callback
void MQTTClient::subscribeSuccessCallbackInvoked(const QString &topicPath) {
    // This is called on a MQTT library thread and not the UI thread so we
    // use a signal to get it back on the proper thread for a messagebox.
    emit subscriptionSuccessSignal(topicPath);
}

// UI thread handler for subscription failures
void MQTTClient::handleSubscriptionSuccess(const QString &topicPath) {
#if 0
    QString infoStr;
    QTextStream infoStream(&infoStr);
    infoStream << "MQTT subscription to topic '" << topicPath
               << "' on broker '" << serverName << "' success.";

    QMessageBox messageBox;
    messageBox.information(NULL, "Subscription Success", infoStr);
#else
    (void)topicPath;
#endif
}

// Static callback invoked by the C MQTT library
void MQTTClient::subscribeFailureCallback(void *context,
                                          MQTTAsync_failureData *response) {
    // Note that this callback is not running on the UI thread, but the method
    // call to topic should be okay since all topics are configured before the
    // connection to the broker is made and any subscriptions sent.
    const Topic *topic = (Topic *)context;
    const QString &topicPath = topic->path();

    const QString message = response->message;

    MQTTClient *client = topic->mqttClient();
    client->subscribeFailureCallbackInvoked(topicPath, message);

    // The Paho example documentation does not free the MQTTAsync_failureData
    // structure so we're currently assuming that, unlike topics, it's freed
    // by the caller. It's work investing further though...
}

// Class method for the above static callback
void MQTTClient::subscribeFailureCallbackInvoked(const QString &topicPath,
                                                 const QString &message) {
    // This is called on a MQTT library thread and not the UI thread so we
    // use a signal to get it back on the proper thread for a messagebox.
    emit subscriptionFailureSignal(topicPath, message);
}

// UI thread handler for subscription failures
void MQTTClient::handleSubscriptionFailure(const QString &topicPath,
                                           const QString message) {
    QString warningStr;
    QTextStream warningStream(&warningStr);
    warningStream << "MQTT subscription to topic '" << topicPath
                  << "' on broker '" << serverName << "' failed:" << Qt::endl;
    warningStream << message;

    QMessageBox messageBox;
    messageBox.warning(NULL, "Subscription Failure", warningStr);
}

void MQTTClient::mqttError(const QString description, int error) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << description << " error for server " << serverName << ":"
                << Qt::endl;
    errorStream << MQTTAsync_strerror(error);

    QMessageBox messageBox;
    messageBox.critical(NULL, "MQTT Error", errorStr);
    exit(EXIT_FAILURE);
}
