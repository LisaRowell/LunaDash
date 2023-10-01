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

#ifndef STRING_INPUT_H
#define STRING_INPUT_H

#include "XMLSourcedEntity.h"

#include <QObject>
#include <QString>
#include <QVector>

class Transformer;
class Variable;
class Variables;
class XMLFileReader;

class StringInput : public QObject, public XMLSourcedEntity {
    Q_OBJECT

private:
    bool isConstant;
    union {
        const Variable *variable;
        const QString *string;
    } input;

    void attributeWarning(const QString &transformerName,
                          const XMLFileReader &xmlReader) const;
    void unknownVariableWarning(const QString &variableName,
                                const XMLFileReader &xmlReader,
                                const QString &transformerName) const;

    static const QVector<QString> allowedAttrs;

public:
    StringInput(XMLFileReader &xmlReader, Variables &variables,
                const QString &transformerName);
    const QString value() const;

public slots:
    void variableChanged();

signals:
    void inputChangedSignal();
};

#endif // STRING_INPUT_H
