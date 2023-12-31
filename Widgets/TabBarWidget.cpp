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

#include "TabBarWidget.h"

#include "TabWidget.h"
#include "Variables.h"
#include "Widget.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"

#include <QString>
#include <QVector>

TabBarWidget::TabBarWidget(XMLFileReader &xmlReader,
                           const Variables &variables,
                           WidgetStyles &widgetStyles) {
    checkAttrs(xmlReader, emptyAttrsList, emptyAttrsList);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Tab") == 0) {
            addTabWidget(xmlReader, variables, widgetStyles);
        } else {
            handleChildElement(xmlReader, "TabBar");
        }
    }
}

void TabBarWidget::addTabWidget(XMLFileReader &xmlReader,
                                const Variables &variables,
                                WidgetStyles &widgetStyles) {
    TabWidget *tabWidget = new TabWidget(xmlReader, variables, widgetStyles);
    (void)addTab(tabWidget, tabWidget->title());
}

