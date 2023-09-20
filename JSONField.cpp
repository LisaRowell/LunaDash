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

#include "JSONField.h"

#include "XMLFileReader.h"
#include "Variables.h"
#include "XMLSourcedEntity.h"
#include "StringVariable.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QJsonValue>
#include <QJsonObject>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> JSONField::allowedAttrs = { "label" };
const QVector<QString> JSONField::requiredAttrs = { "label" };

JSONField::JSONField(XMLFileReader &xmlReader, Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), type(QJsonValue::Undefined) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, xmlReader);
    label = attributes.value("label").toString();

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name().compare("String") == 0) {
            addStringVariable(xmlReader, variables);
        } else {
            unsupportedChildElement("Field", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

void JSONField::addStringVariable(XMLFileReader &xmlReader,
                                  Variables &variables) {
    // Make sure we only are working with one variable for each field
    if (type == QJsonValue::Undefined) {
        StringVariable *variable = new StringVariable(xmlReader, variables);
        variables.addVariable(variable);
        connect(this, &JSONField::stringFieldChangedSignal,
                variable, &StringVariable::newValue);
        type = QJsonValue::String;
    } else {
        multipleVariableWarning(xmlReader);
        xmlReader.skipCurrentElement();
    }
}

void JSONField::objectReceived(QJsonObject &object) {
    // Each field gets the JSON object and is responsible for pulling its
    // field out if it exists.
    if (object.contains(label)) {
        QJsonValue value = object.value(label);

        // We handle values differently based on what type of field this is.
        switch (type) {
        case QJsonValue::String:
            receivedValueForString(value);
            break;

        default:
            // We received a JSON field type that we don't currently support,
            // consider adding some sort of debug here.
            break;
        }
    } else {
        // We received a  MQTT publish for a JSON encoded topic, but the
        // defined field. This might me a good thing to have a debug message
        // for.
    }
}

void JSONField::receivedValueForString(QJsonValue &value) {
    switch (value.type()) {
    case QJsonValue::String:
        emit stringFieldChangedSignal(value.toString());
        break;

    case QJsonValue::Double:
        {
            QString stringValue = QString::number(value.toDouble());
            emit stringFieldChangedSignal(stringValue);
        }
        break;

    case QJsonValue::Bool:
        if (value.toBool() == true) {
            emit stringFieldChangedSignal("true");
        } else {
            emit stringFieldChangedSignal("false");
        }
        break;

    default:
        // We have a new value for a field which we can't convert to a string.
        // Add a debug?
        emit stringFieldChangedSignal("");
        break;
    }
}

void JSONField::multipleVariableWarning(XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "JSON field '" << label
                  << "' has multiple variables in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignored all but the first. "
                  << "Consider using multiple fields with the same label.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Multiple JSON Field Variables", warningStr);
}
