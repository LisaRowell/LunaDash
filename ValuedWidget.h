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

#ifndef VALUED_WIDGET_H
#define VALUED_WIDGET_H

#include "Widget.h"
#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QObject>

class ValuedWidget : public QObject, public Widget {
    Q_OBJECT

private:
    void unknownVariableWarning(const QString &widgetType,
                                const QString &variableName,
                                const XMLFileReader &xmlReader) const;

    static const QVector<QString> baseAllowedAttrs;
    static const QVector<QString> baseRequiredAttrs;

protected:
    Variable *variable;

    virtual void setValue() = 0;

public:
    ValuedWidget(const QString &widgetType, XMLFileReader &xmlReader,
                 const Variables &variables,
                 const QVector<QString> allowedAttrs = baseAllowedAttrs,
                 const QVector<QString> requiredAttrs = baseRequiredAttrs);

public slots:
    void valueChanged();
};

#endif // VALUED_WIDGET_H
