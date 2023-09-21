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

#ifndef BOOL_VARIABLE_H
#define BOOL_VARIABLE_H

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>
#include <QObject>

class BoolVariable : public Variable {
    Q_OBJECT

private:
    bool value_;

public:
    BoolVariable(XMLFileReader &xmlReader, Variables &variables);
    void set(bool value);
    virtual const QString string() const override;
    virtual double doubleValue(bool *valid) const override;
    virtual bool boolValue(bool *valid) const override;

public slots:
    void newStringValue(const QString &value);
    void newBoolValue(bool value);
    void resetValue();
};

#endif // BOOL_VARIABLE_H
