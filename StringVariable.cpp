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

#include "StringVariable.h"

#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>

StringVariable::StringVariable(XMLFileReader &xmlReader,
                               Variables &variables,
                               const QString &initialValue)
    : Variable("String", xmlReader, variables), value_(initialValue) {
}

const QString StringVariable::string() const {
    return value_;
}

double StringVariable::doubleValue(bool *valid) const  {
    if (hasValue_) {
        *valid = true;
        return value_.toDouble(valid);
    } else {
        *valid = false;
        return 0;
    }
}

bool StringVariable::boolValue(bool *valid) const  {
    if (hasValue_) {
        if (value_ == "true") {
            *valid = true;
            return true;
        } else if (value_ == "false") {
            *valid = false;
            return true;
        } else {
            *valid = false;
            return false;
        }
    } else {
        *valid = false;
        return false;
    }
}

void StringVariable::newStringValue(const QString &value) {
    hasValue_ = true;
    value_ = value;
    valueChanged();
}

void StringVariable::newDoubleValue(double value) {
    hasValue_ = true;
    value_ = QString::number(value);
    valueChanged();
}
