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

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "XMLSourcedEntity.h"

#include "Variables.h"
#include "WidgetStyles.h"
#include "GridPos.h"
#include "XMLFileReader.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QStringView>

class Dashboard : public QMainWindow, public XMLSourcedEntity {
    Q_OBJECT

private:
    Variables variables;
    WidgetStyles widgetStyles;
    QWidget *mainWidget;
    QGridLayout *layout;

    void initWindow();
    void createShortcuts();
    void addLabelWidget(XMLFileReader &xmlReader);
    void addTextWidget(XMLFileReader &xmlReader);
    void addNumberWidget(XMLFileReader &xmlReader);
    void addBoolWidget(XMLFileReader &xmlReader);
    void addLabelIndicatorWidget(XMLFileReader &xmlReader);
    void addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                           const QString &typeName, XMLFileReader &xmlReader);
    void missingGridPosWarning(const QString &widgetType,
                               const XMLFileReader &xmlReader) const;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

public:
    Dashboard(XMLFileReader &xmlReader, QWidget *parent = nullptr);
    ~Dashboard();

public slots:
    void exit();
};

#endif // DASHBOARD_H
