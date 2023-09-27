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

#include "LabelWidget.h"

#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QString>
#include <QVector>

const QVector<QString> LabelWidget::allowedAttrs = { "text" };
const QVector<QString> LabelWidget::requiredAttrs = { "text" };

LabelWidget::LabelWidget(XMLFileReader &xmlReader)
    : Widget(allowedAttrs, requiredAttrs) {
    // We should have just a single 'text' attribute.
    checkAttrs(xmlReader);
    const QString label = stringAttribute("text", xmlReader);
    setText(label);

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Label");
    }
}
