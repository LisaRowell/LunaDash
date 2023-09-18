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

#include "Topic.h"

#include "XMLSourcedEntity.h"
#include "Variables.h"
#include "Variable.h"
#include "StringVariable.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>

const QVector<QString> Topic::allowedAttrs = { "path" };
const QVector<QString> Topic::requiredAttrs = { "path" };

Topic::Topic(QXmlStreamReader &xmlReader, const QString &fileName,
             Variables &variables, MQTTClient *mqttClient)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), mqttClient_(mqttClient) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    path_ = attributes.value("path").toString();

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name().compare("String") == 0) {
            addStringVariable(xmlReader, fileName, variables);
        } else {
            unsupportedChildElement("Topic", fileName, xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

void Topic::addStringVariable(QXmlStreamReader &xmlReader,
                              const QString &fileName,
                              Variables &variables) {
    StringVariable *variable = new StringVariable(xmlReader, fileName,
                                                  variables);
    variables.addVariable(variable);
    connect(this, &Topic::messageReceivedSignal,
            variable, &StringVariable::newValue);
}

const QString &Topic::path() const {
    return path_;
}

MQTTClient *Topic::mqttClient() const {
    return mqttClient_;
}

void Topic::messageReceived(const QString &message) {
    emit messageReceivedSignal(message);
}
