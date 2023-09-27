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

#ifndef VARIABLE_H
#define VARIABLE_H

#include "XMLSourcedEntity.h"

#include <QObject>
#include <QString>
#include <QVector>

class XMLFileReader;
class Variables;

class Variable : public QObject, public XMLSourcedEntity {
    Q_OBJECT;

private:
    QString name_;

    [[noreturn]] void
    duplicateVariableNamesErrors(const QString &name,
                                  const XMLFileReader &xmlReader) const;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

protected:
    bool hasValue_;
    void valueChanged();

public:
    Variable(const QString &typeName, XMLFileReader &xmlReader,
             Variables &variables);
    const QString &name() const;
    bool hasValue() const;
    virtual const QString string() const = 0;
    virtual double doubleValue(bool *valid) const = 0;
    virtual bool boolValue(bool *valid) const = 0;

signals:
    void valueChangedSignal();
};

#endif // VARIABLE_H
