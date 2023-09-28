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

#include "Adder.h"

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>
#include <QVector>

const QVector<QString> Adder::allowedAttrs = { };
const QVector<QString> Adder::requiredAttrs = { };

Adder::Adder(XMLFileReader &xmlReader, Variables &variables)
    : Transformer(allowedAttrs, requiredAttrs){
    checkAttrs(xmlReader);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Addend") == 0) {
            addAddend(xmlReader, variables);
        } else {
            handleChildElement(elementName, "StringDecoder", xmlReader,
                               variables);
        }
    }
}

void Adder::addAddend(XMLFileReader &xmlReader, Variables &variables) {
    Variable *addendVariable = findSourceVariable("variable", "Adder",
                                                  xmlReader, variables);
    addendVariables.append(addendVariable);
    xmlReader.skipCurrentElement();
}

void Adder::recalculate() {
    double result = 0.0;

    foreach (const Variable *variableAddend, addendVariables) {
        bool valid;
        const double value = variableAddend->doubleValue(&valid);
        if (valid) {
            result += value;
        }
    }

    publishResult(result);
}
