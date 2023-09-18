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

#include <QXmlStreamReader>
#include <QString>

StringVariable::StringVariable(QXmlStreamReader &xmlReader,
                               const QString &fileName,
                               Variables &variables,
                               const QString &initialValue)
    : Variable("String", xmlReader, fileName, variables), value_(initialValue) {
}

const QString &StringVariable::string() const {
    return value_;
}

void StringVariable::newValue(const QString value) {
    value_ = value;
    valueChanged();
}
