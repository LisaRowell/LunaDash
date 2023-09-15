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

#include <QVector>
#include <QString>

const QVector<QString> GridPos::allowedAttrs = { "row", "col" };
const QVector<QString> GridPos::requiredAttrs = { "row", "col" };

GridPos::GridPos()
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), set(false), _row(0),
    _col(0) {
}

void GridPos::setFromXML(QXmlStreamReader &xmlReader,
                         const QString &fileName) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    _row = attributes.value("row").toUInt();
    _col = attributes.value("col").toUInt();
    set = true;

    ignoreChildElements(xmlReader, "GridPos", fileName);
}

bool GridPos::isSet() const {
    return set;
}

unsigned GridPos::row() const {
    return _row;
}

unsigned GridPos::col() const {
    return _col;
}
