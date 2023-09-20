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

#include "JSONDecoder.h"

#include "XMLFileReader.h"
#include "Variables.h"
#include "JSONField.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

const QVector<QString> JSONDecoder::allowedAttrs = { };
const QVector<QString> JSONDecoder::requiredAttrs = { };

JSONDecoder::JSONDecoder(XMLFileReader &xmlReader, Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, xmlReader);

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name().compare("Field") == 0) {
            addField(xmlReader, variables);
        } else {
            unsupportedChildElement("JSON", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

void JSONDecoder::addField(XMLFileReader &xmlReader, Variables &variables) {
    JSONField *field = new JSONField(xmlReader, variables);

    fields.append(field);
}

void JSONDecoder::newEncoding(const QString &value) {
    // This isn't great efficency wise. Consider looking into removing the
    // copy.
    QByteArray valueArray = value.toUtf8();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(valueArray);

    if (!jsonDocument.isNull()) {
        QJsonObject jsonObject = jsonDocument.object();
        for (JSONField *field : fields) {
            field->objectReceived(jsonObject);
        }
    } else {
        // Bad JSON encoding, add some sort of debug to help the user.
    }
}
