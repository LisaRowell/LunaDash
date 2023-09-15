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

#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include <QObject>

class ValuedWidget : public Widget {
    Q_OBJECT

private:
    void unknownVariableWarning(const QString &widgetType,
                                const QString &variableName,
                                const QXmlStreamReader &xmlReader,
                                const QString &fileName) const;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

protected:
    Variable *variable;

    virtual void setValue() = 0;

public:
    ValuedWidget(const QString &widgetType, QXmlStreamReader &xmlReader,
                 const QString &fileName, const Variables &variables);

public slots:
    void valueChanged();
};

#endif // VALUED_WIDGET_H
