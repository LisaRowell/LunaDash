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

#include "Variables.h"
#include "GridPos.h"
#include "XMLFileReader.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QStringView>

class Dashboard : public QMainWindow {
    Q_OBJECT

private:
    Variables variables;
    QWidget *mainWidget;
    QGridLayout *layout;

    void initWindow();
    void readConfig(const QString &configFileName, Variables &variables);
    void readConfigStart(XMLFileReader &xmlReader);
    void addLabel(XMLFileReader &xmlReader);
    void addText(XMLFileReader &xmlReader);
    void addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                           XMLFileReader &xmlReader);
    [[noreturn]] void xmlParseError(const XMLFileReader &xmlReader) const;
    [[noreturn]] void
    xmlDocumentStartError(const XMLFileReader &xmlReader) const;
    [[noreturn]] void
    xmlDocumentTypeError(const XMLFileReader &xmlReader) const;
    void unknownElementWarning(const QString &parentName,
                               const XMLFileReader &xmlReader) const;
    void missingGridPosWarning(const QString &widgetType,
                               const XMLFileReader &xmlReader) const;

public:
    Dashboard(const QString &configFileName, QWidget *parent = nullptr);
    ~Dashboard();
};

#endif // DASHBOARD_H
