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

#ifndef WIDGET_STYLES_H
#define WIDGET_STYLES_H

#include <QMap>
#include <QString>

class WidgetStyle;

class WidgetStyles {
private:
    QMap<QString, WidgetStyle *> styles;

public:
    WidgetStyles();
    ~WidgetStyles();
    bool styleExists(const QString &name) const;
    void addStyle(WidgetStyle *style);
    WidgetStyle *find(const QString &name) const;
};

#endif // WIDGET_STYLES_H
