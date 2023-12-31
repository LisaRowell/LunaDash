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

#ifndef BOX_WIDGET_H
#define BOX_WIDGET_H

#include "Widget.h"

#include <QGroupBox>

#include <QObject>
#include <QString>
#include <QVector>

class Variable;
class Variables;
class WidgetGrid;
class WidgetStyles;
class XMLFileReader;

class BoxWidget : public QGroupBox, public Widget {
    Q_OBJECT

private:
    WidgetGrid *layout;
    bool expandable_;
    Variable *labelVariable;

    void initTitle(const XMLFileReader &xmlReader,
                   const Variables &variables);
    void labelAndLabelVariableWarning(const XMLFileReader &xmlReader) const;
    void unknownLabelVariableWarning(const QString &variableName,
                                     const XMLFileReader &xmlReader) const;

    static const QVector<QString> allowedAttrs;
    static const QString className;

public:
    BoxWidget(XMLFileReader &xmlReader, const Variables &variables,
              WidgetStyles &widgetStyles);
    bool expandable() const;

public slots:
    void updateTitle();
};

#endif // BOX_WIDGET_H
