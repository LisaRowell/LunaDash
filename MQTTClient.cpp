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

#include <MQTTAsync.h>

#include <QXmlStreamReader>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QMessageBox>
#include <QObject>

const QVector<QString> MQTTClient::allowedAttrs = { "server" };
const QVector<QString> MQTTClient::requiredAttrs = { "server" };

MQTTClient::MQTTClient(QXmlStreamReader &xmlReader, const QString &fileName,
                       Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    connect(this, SIGNAL(connectedSignal()), this, SLOT(connected()));
    connect(this, SIGNAL(connectionLostSignal()), this, SLOT(disconnected()));

    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    serverName = attributes.value("server").toString();

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name().compare("StatusVariable") == 0) {
            addStatusVariable(xmlReader, fileName, variables);
        } else {
            unsupportedChildElement("MQTTBroker", fileName, xmlReader);
            xmlReader.skipCurrentElement();
        }
    }

    startConnection();
}

void MQTTClient::startConnection() {
    const QString serverURI = "tcp://" + serverName + ":1883";
    const QByteArray localServerURI = serverURI.toLocal8Bit();

    int error;
    error = MQTTAsync_create(&handle, localServerURI.data(), "LunaDash",
                             MQTTCLIENT_PERSISTENCE_NONE, NULL);
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

int MQTTClient::messageArrivedCallback(void *context, char *topicName,
                                       int topicLen,
                                       MQTTAsync_message *message) {
    MQTTClient *client = (MQTTClient *)context;

    // ***** To be implemented *****
    (void)client;
    (void)topicName;
    (void)topicLen;
    (void)message;

    return true;
}

void MQTTClient::deliveryCompleteCallback(void *context,
                                          MQTTAsync_token token) {
    (void)context;
    (void)token;
}

void MQTTClient::addStatusVariable(QXmlStreamReader &xmlReader,
                                   const QString &fileName,
                                   Variables &variables) {
    ClientStatusVariable *statusVariable =
        new ClientStatusVariable(xmlReader, fileName, variables);
    variables.addVariable(statusVariable);
    statusVariables.append(statusVariable);
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
