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

#include "XMLSourcedEntity.h"
#include "XMLFileReader.h"

#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QVector>
#include <QString>

const QVector<QString> LabelWidget::allowedAttrs = { "text" };
const QVector<QString> LabelWidget::requiredAttrs = { "text" };

LabelWidget::LabelWidget(XMLFileReader &xmlReader)
    : Widget(allowedAttrs, requiredAttrs) {
    // We should have just a single 'text' attribute.
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, xmlReader);
    setText(attributes.value("text").toString());

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Label");
    }
}
