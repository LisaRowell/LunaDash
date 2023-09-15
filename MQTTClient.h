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

#include <MQTTAsync.h>

#include <QXmlStreamReader>
#include <QVector>
#include <QString>
#include <QObject>

class MQTTClient : public XMLSourcedEntity {
    Q_OBJECT

private:
    QVector<ClientStatusVariable *>statusVariables;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    QString serverName;
    MQTTAsync handle;

    void startConnection();
    void connectedCallbackInvoked();
    void connectionLostCallbackInvoked();
    void addStatusVariable(QXmlStreamReader &xmlReader,
                           const QString &fileName, Variables &variables);
    void mqttError(const QString description, int error) const;

    static void connectedCallback(void *context, char *cause);
    static void connectionLostCallback(void *context, char *cause);
    static int messageArrivedCallback(void *context, char *topicName,
                                      int topicLen,
                                      MQTTAsync_message *message);
    static void deliveryCompleteCallback(void *context, MQTTAsync_token token);

public slots:
    void connected();
    void disconnected();

signals:
    void connectedSignal();
    void connectionLostSignal();

public:
    MQTTClient(QXmlStreamReader &xmlReader, const QString &fileName,
               Variables &variables);
};

#endif // MQTT_CLIENT_H
