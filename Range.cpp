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

#include "Range.h"

#include "XMLSourcedEntity.h"

#include <QVector>
#include <QString>

const QVector<QString> Range::allowedAttrs = { "min", "max" };
const QVector<QString> Range::requiredAttrs = { "min", "max" };

Range::Range() : XMLSourcedEntity(allowedAttrs, requiredAttrs), set_(false) {
}

void Range::set(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader);

    set_ = true;
    bool minValid;
    min_ = doubleAttribute("min", xmlReader, &minValid);
    bool maxValid;
    max_ = doubleAttribute("max", xmlReader, &maxValid);
    set_ = minValid && maxValid;

    ignoreChildElements(xmlReader, "Range");
}

bool Range::isSet() const {
    return set_;
}

double Range::min() const {
    return min_;
}

double Range::max() const {
    return max_;
}
