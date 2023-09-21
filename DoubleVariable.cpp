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

#include "DoubleVariable.h"
#include "XMLFileReader.h"
#include "Variables.h"

#include <QString>

DoubleVariable::DoubleVariable(XMLFileReader &xmlReader,
                               Variables &variables)
    : Variable("Double", xmlReader, variables){
}

const QString DoubleVariable::string() const {
    if (hasValue_) {
        return QString::number(value_);
    } else {
        return "";
    }
}

double DoubleVariable::doubleValue(bool *valid) const  {
    if (hasValue_) {
        *valid = true;
        return value_;
    } else {
        *valid = false;
        return 0;
    }
}

bool DoubleVariable::boolValue(bool *valid) const  {
    if (hasValue_) {
        *valid = true;
        return value_ != 0;
    } else {
        *valid = false;
        return false;
    }
}

void DoubleVariable::newStringValue(const QString &value) {
    value_ = value.toDouble(&hasValue_);
    valueChanged();
}

void DoubleVariable::newDoubleValue(double value) {
    hasValue_ = true;
    value_ = value;
    valueChanged();
}

void DoubleVariable::resetValue() {
    hasValue_ = false;
    valueChanged();
}
