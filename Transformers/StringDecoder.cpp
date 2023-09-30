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

#include "StringDecoder.h"

#include "Transformer.h"
#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>
#include <QVector>

const QVector<QString> StringDecoder::allowedAttrs = { "string" };
const QVector<QString> StringDecoder::requiredAttrs = { "string" };

StringDecoder::StringDecoder(XMLFileReader &xmlReader, Variables &variables) {
    checkAttrs(xmlReader, allowedAttrs, requiredAttrs);
    inputVariable = findSourceVariable("string", "StringDecoder", xmlReader,
                                       variables);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Case") == 0) {
            addCase(xmlReader);
        } else if (elementName.compare("Default") == 0) {
            addDefault(xmlReader);
        } else {
            handleChildElement(elementName, "StringDecoder", xmlReader,
                               variables);
        }
    }
}

void StringDecoder::addCase(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader, { "value", "result" }, { "value", "result" });

    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    const QStringView valueAttribute = attributes.value("value");
    const QString value = valueAttribute.toString();
    const QStringView resultAttribute = attributes.value("result");
    const QString result = resultAttribute.toString();
    cases.insert(value, result);

    ignoreChildElements(xmlReader, "Case");
}

void StringDecoder::addDefault(XMLFileReader &xmlReader) {
    defaultValue = stringElement("result", xmlReader);
}

void StringDecoder::recalculate() {
    if (inputVariable) {
        const QString &result = cases.value(inputVariable->string(), defaultValue);
        publishResult(result);
    } else {
        // It shouldn't be possible to not have an inputVariable and still
        // get called to recalculate.
    }
}
