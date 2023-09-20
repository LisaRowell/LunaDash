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

#ifndef TOPIC_H
#define TOPIC_H

#include "XMLSourcedEntity.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QObject>
#include <QVector>
#include <QString>

class MQTTClient;

class Topic : public QObject, public XMLSourcedEntity {
    Q_OBJECT

private:
    QString path_;
    MQTTClient *mqttClient_;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    void addStringVariable(XMLFileReader &xmlReader, Variables &variables);
    void addDoubleVariable(XMLFileReader &xmlReader, Variables &variables);
    void addJSONDecoder(XMLFileReader &xmlReader, Variables &variables);

public:
    Topic(XMLFileReader &xmlReader, Variables &variables,
          MQTTClient *mqttClient);
    const QString &path() const;
    MQTTClient *mqttClient() const;
    void messageReceived(const QString &string);

signals:
    void messageReceivedSignal(QString message);
};

#endif // TOPIC_H
