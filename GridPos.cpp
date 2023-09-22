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

#include <QVector>
#include <QString>

const QVector<QString> GridPos::allowedAttrs = { "row", "col" };
const QVector<QString> GridPos::requiredAttrs = { "row", "col" };

GridPos::GridPos(XMLFileReader &xmlReader)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    checkAttrs(xmlReader);
    _row = unsignedAttribute("row", xmlReader);
    _col = unsignedAttribute("col", xmlReader);

    ignoreChildElements(xmlReader, "GridPos");
}

unsigned GridPos::row() const {
    return _row;
}

unsigned GridPos::col() const {
    return _col;
}
