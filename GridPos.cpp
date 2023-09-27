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

#include "GridPos.h"

#include "XMLFileReader.h"

#include <QString>
#include <QVector>

const QVector<QString> GridPos::allowedAttrs = {
    "row", "col", "rowSpan", "colSpan"
};
const QVector<QString> GridPos::requiredAttrs = { "row", "col" };

GridPos::GridPos(XMLFileReader &xmlReader)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    checkAttrs(xmlReader);
    row_ = unsignedAttribute("row", xmlReader);
    col_ = unsignedAttribute("col", xmlReader);
    rowSpan_ = unsignedAttribute("rowSpan", xmlReader, 1);
    colSpan_ = unsignedAttribute("colSpan", xmlReader, 1);

    ignoreChildElements(xmlReader, "GridPos");
}

unsigned GridPos::row() const {
    return row_;
}

unsigned GridPos::col() const {
    return col_;
}

unsigned GridPos::rowSpan() const {
    return rowSpan_;
}

unsigned GridPos::colSpan() const {
    return colSpan_;
}
