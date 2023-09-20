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

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "XMLSourcedEntity.h"
#include "Variables.h"
#include "ClientStatusVariable.h"
#include "Topic.h"
#include "XMLFileReader.h"

#include <MQTTAsync.h>

#include <QVector>
#include <QString>
#include <QObject>
#include <QMap>

class MQTTClient : public QObject, public XMLSourcedEntity {
    Q_OBJECT

private:
    QString serverName;
    MQTTAsync handle;
    QVector<ClientStatusVariable *> statusVariables;
    QMap<QString, Topic *> topics;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    void startConnection();
    void connectedCallbackInvoked();
    void connectionLostCallbackInvoked();
    void messageArrivedCallbackInvoked(const QString &topicPath,
                                       const QString &payload);
    void addStatusVariable(XMLFileReader &xmlReader, Variables &variables);
    void addTopic(XMLFileReader &xmlReader, Variables &variables);
    void subscribeToTopics() const;
    void subscribe(const Topic *topic) const;
    void subscribeSuccessCallbackInvoked(const QString &topicPath);
    void subscribeFailureCallbackInvoked(const QString &topicPath,
                                         const QString &message);
    void mqttError(const QString description, int error) const;

    static void connectedCallback(void *context, char *cause);
    static void connectionLostCallback(void *context, char *cause);
    static int messageArrivedCallback(void *context, char *topicName,
                                      int topicLen,
                                      MQTTAsync_message *message);
    static void deliveryCompleteCallback(void *context,
                                         MQTTAsync_token token);
    static void subscribeSuccessCallback(void *context,
                                         MQTTAsync_successData *response);
    static void subscribeFailureCallback(void *context,
                                         MQTTAsync_failureData *response);

public:
    MQTTClient(XMLFileReader &xmlReader, Variables &variables);

public slots:
    void connected();
    void disconnected();
    void handleSubscriptionSuccess(const QString &topicPath);
    void handleSubscriptionFailure(const QString &topicPath,
                                   const QString message);
    void messageForUnknownTopic(const QString &topicPath);

signals:
    void connectedSignal();
    void connectionLostSignal();
    void subscriptionSuccessSignal(const QString topicPath);
    void subscriptionFailureSignal(const QString topicPath,
                                   const QString message);
    void messageForUnknownTopicSignal(const QString topicPath);
};

#endif // MQTT_CLIENT_H
