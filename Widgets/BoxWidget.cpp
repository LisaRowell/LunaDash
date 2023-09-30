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

#include "BoxWidget.h"

#include "Variables.h"
#include "WidgetGrid.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"

#include <QString>
#include <QStringView>
#include <QWidget>
#include <QVector>

const QVector<QString> BoxWidget::allowedAttrs = { "label", "flat" };
const QString BoxWidget::className = "Box";

BoxWidget::BoxWidget(XMLFileReader &xmlReader, const Variables &variables,
                     WidgetStyles &widgetStyles) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);
    const QString label = stringAttribute("label", xmlReader);
    if (!label.isEmpty()) {
        setTitle(label);
    }
    const bool flat = boolAttribute("flat", xmlReader, false);
    if (flat) {
        setFlat(true);
        setStyleSheet("QGroupBox {border:0;}");
        expandable_ = false;
    } else {
        expandable_ = true;
    }

    layout = new WidgetGrid(className);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        const bool handled = layout->handleXMLElement(elementName, xmlReader,
                                                      variables,
                                                      widgetStyles);
        if (!handled) {
            handleChildElement(xmlReader, "Box");
        }
    }

    setLayout(layout);
}

bool BoxWidget::expandable() const {
    return expandable_;
}
