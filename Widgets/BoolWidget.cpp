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

#include "BoolWidget.h"

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

const QVector<QString> BoolWidget::additionalAllowedAttrs = {
    "true", "false"
};

BoolWidget::BoolWidget(XMLFileReader &xmlReader, const Variables &variables)
    : ValuedWidget("Bool", xmlReader, variables, additionalAllowedAttrs,
                   emptyAttrsList) {
    trueValue = stringAttribute("true", xmlReader, "true");
    falseValue = stringAttribute("false", xmlReader, "false");
    BoolWidget::setValue();

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Bool");
    }
}

void BoolWidget::setValue() {
    if (variable != NULL && variable->hasValue()) {
        bool valid;
        bool value = variable->boolValue(&valid);
        if (valid) {
            if (value) {
                setText(trueValue);
            } else {
                setText(falseValue);
            }
        } else {
            setText("");
        }
    } else {
        setText("");
    }
}
