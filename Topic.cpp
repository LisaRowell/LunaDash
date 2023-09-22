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
#include "DoubleVariable.h"
#include "BoolVariable.h"
#include "JSONDecoder.h"
#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QObject>

const QVector<QString> Topic::allowedAttrs = { "path" };
const QVector<QString> Topic::requiredAttrs = { "path" };

Topic::Topic(XMLFileReader &xmlReader, Variables &variables,
             MQTTClient *mqttClient)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), mqttClient_(mqttClient) {
    checkAttrs(xmlReader);
    path_ = stringAttribute("path", xmlReader);

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name().compare("String") == 0) {
            addStringVariable(xmlReader, variables);
        } else if (xmlReader.name().compare("Double") == 0) {
            addDoubleVariable(xmlReader, variables);
        } else if (xmlReader.name().compare("Bool") == 0) {
            addBoolVariable(xmlReader, variables);
        } else if (xmlReader.name().compare("JSON") == 0) {
            addJSONDecoder(xmlReader, variables);
        } else {
            unsupportedChildElement("Topic", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

void Topic::addStringVariable(XMLFileReader &xmlReader,
                              Variables &variables) {
    StringVariable *variable = new StringVariable(xmlReader, variables);
    variables.addVariable(variable);
    connect(this, &Topic::messageReceivedSignal,
            variable, &StringVariable::newValue);
}

void Topic::addDoubleVariable(XMLFileReader &xmlReader,
                              Variables &variables) {
    DoubleVariable *variable = new DoubleVariable(xmlReader, variables);
    variables.addVariable(variable);
    connect(this, &Topic::messageReceivedSignal,
            variable, &DoubleVariable::newStringValue);
}

void Topic::addBoolVariable(XMLFileReader &xmlReader, Variables &variables) {
    BoolVariable *variable = new BoolVariable(xmlReader, variables);
    variables.addVariable(variable);
    connect(this, &Topic::messageReceivedSignal,
            variable, &BoolVariable::newStringValue);
}

void Topic::addJSONDecoder(XMLFileReader &xmlReader, Variables &variables) {
    JSONDecoder *jsonDecoder = new JSONDecoder(xmlReader, variables);
    connect(this, &Topic::messageReceivedSignal,
            jsonDecoder, &JSONDecoder::newEncoding);
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
