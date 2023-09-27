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

#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include "XMLSourcedEntity.h"

#include <QWidget>

class Variables;
class WidgetGrid;
class WidgetStyles;
class XMLFileReader;

class TabWidget : public QWidget, public XMLSourcedEntity {
private:
    WidgetGrid *layout;
    QString title_;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;
    static const QString className;

public:
    TabWidget(XMLFileReader &xmlReader, const Variables &variables,
              WidgetStyles &widgetStyles);
    const QString &title() const;
};

#endif // TAB_WIDGET_H
