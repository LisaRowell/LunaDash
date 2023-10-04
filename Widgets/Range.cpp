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

#include <QString>
#include <QVector>

const QVector<QString> Range::allowedAttrs = { "min", "max" };
const QVector<QString> Range::requiredAttrs = { "min", "max" };

Range::Range(double min, double max) : min_(min), max_(max) {
}

void Range::set(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader, allowedAttrs, requiredAttrs);

    bool minValid;
    const double newMin = doubleAttribute("min", xmlReader, &minValid);
    if (minValid) {
        min_ = newMin;
    }

    bool maxValid;
    const double newMax = doubleAttribute("max", xmlReader, &maxValid);
    if (maxValid) {
        max_ = newMax;
    }

    ignoreChildElements(xmlReader, "Range");
}

double Range::min() const {
    return min_;
}

double Range::max() const {
    return max_;
}
