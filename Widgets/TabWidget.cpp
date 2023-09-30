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

#include "TabWidget.h"

#include "Variables.h"
#include "WidgetGrid.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QString>
#include <QVector>

const QVector<QString> TabWidget::allowedAttrs = { "title" };
const QVector<QString> TabWidget::requiredAttrs = { "title" };
const QString TabWidget::className = "Tab";

TabWidget::TabWidget(XMLFileReader &xmlReader, const Variables &variables,
                     WidgetStyles &widgetStyles) {
    checkAttrs(xmlReader, allowedAttrs, requiredAttrs);
    title_ = stringAttribute("title", xmlReader);

    layout = new WidgetGrid(className);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        const bool handled = layout->handleXMLElement(elementName, xmlReader,
                                                      variables,
                                                      widgetStyles);
        if (!handled) {
            unsupportedChildElement("Tab", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }

    setLayout(layout);
}

const QString &TabWidget::title() const {
    return title_;
}
