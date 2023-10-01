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

#include "Concatenator.h"

#include "StringInput.h"
#include "XMLFileReader.h"

#include <QStringView>

Concatenator::Concatenator(XMLFileReader &xmlReader, Variables &variables) {
    checkAttrs(xmlReader, emptyAttrsList, emptyAttrsList);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Input") == 0) {
            addInput(xmlReader, variables);
        } else {
            handleChildElement(elementName, "Concatenator", xmlReader,
                               variables);
        }
    }

    Concatenator::recalculate();
}

void Concatenator::addInput(XMLFileReader &xmlReader, Variables &variables) {
    StringInput *input = new StringInput(xmlReader, variables,
                                         "Concatenator");
    inputs.append(input);

    connect(input, &StringInput::inputChangedSignal,
            this, &Transformer::inputChanged);
}

void Concatenator::recalculate() {
    QString result;

    foreach (const StringInput *input, inputs) {
        result.append(input->value());
    }

    publishResult(result);
}
