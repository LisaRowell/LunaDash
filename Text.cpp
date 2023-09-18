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

#include "Text.h"

#include "ValuedWidget.h"
#include "Variable.h"
#include "Variables.h"

#include <QVector>
#include <QString>

const QString Text::typeName("Text");

Text::Text(QXmlStreamReader &xmlReader, const QString &fileName,
           const Variables &variables)
    : ValuedWidget(typeName, xmlReader, fileName, variables) {

    if (variable != NULL) {
        setText(variable->string());
    } else {
        setText("");
    }

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Text", fileName);
    }
}

void Text::setValue() {
    if (variable != NULL) {
        setText(variable->string());
    }
}