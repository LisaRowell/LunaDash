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

#ifndef WIDGET_GRID_H
#define WIDGET_GRID_H

#include "XMLFileReader.h"
#include "GridPos.h"
#include "Variables.h"
#include "WidgetStyles.h"

#include <QGridLayout>
#include <QString>
#include <QWidget>

class WidgetGrid : public QGridLayout {
private:
    const QString &containerName;

    void addLabelWidget(XMLFileReader &xmlReader);
    void addTextWidget(XMLFileReader &xmlReader, const Variables &variables);
    void addNumberWidget(XMLFileReader &xmlReader,
                         const Variables &variables);
    void addBoolWidget(XMLFileReader &xmlReader, const Variables &variables);
    void addLabelIndicatorWidget(XMLFileReader &xmlReader,
                                 const Variables &variables,
                                 WidgetStyles &widgetStyles);
    void addBoxWidget(XMLFileReader &xmlReader, const Variables &variables,
                      WidgetStyles &widgetStyles);
    void addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                           const QString &typeName, XMLFileReader &xmlReader);
    void missingGridPosWarning(const QString &widgetType,
                               const XMLFileReader &xmlReader) const;

public:
    WidgetGrid(const QString &containerName, QWidget *parent = nullptr);
    bool handleXMLElement(const QStringView &name,
                          XMLFileReader &xmlReader,
                          const Variables &variables,
                          WidgetStyles &widgetStyles);
};

#endif // WIDGET_GRID_H
