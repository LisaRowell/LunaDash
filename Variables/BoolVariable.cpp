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

#include "BoolVariable.h"

#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>

BoolVariable::BoolVariable(XMLFileReader &xmlReader, Variables &variables)
    : Variable("BoolVariable", xmlReader, variables) {
}

void BoolVariable::set(bool value) {
    hasValue_ = true;
    value_ = value;
    valueChanged();
}

const QString BoolVariable::string() const {
    if (hasValue_) {
        if (value_) {
            return "true";
        } else {
            return "false";
        }
    } else {
        return "";
    }
}

double BoolVariable::doubleValue(bool *valid) const  {
    if (hasValue_) {
        *valid = true;
        if (value_) {
            return 1;
        } else {
            return 0;
        }
    } else {
        *valid = false;
        return 0;
    }
}

bool BoolVariable::boolValue(bool *valid) const  {
    if (hasValue_) {
        *valid = true;
        return value_;
    } else {
        *valid = false;
        return false;
    }
}

void BoolVariable::newStringValue(const QString &value) {
    if (value == "true" || value == "1") {
        value_ = true;
        hasValue_ = true;
    } else if (value == "false" || value == "0") {
        value_ = false;
        hasValue_ = true;
    } else {
        hasValue_ = false;
    }
    valueChanged();
}

void BoolVariable::newBoolValue(bool value) {
    value_ = value != 0;
    hasValue_ = true;
    valueChanged();
}

void BoolVariable::resetValue() {
    hasValue_ = false;
    valueChanged();
}
