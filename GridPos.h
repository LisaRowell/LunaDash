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

#ifndef GRID_POS_H
#define GRID_POS_H

#include "XMLSourcedEntity.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>

class GridPos : public XMLSourcedEntity {
private:
    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;
    bool set;
    unsigned _row;
    unsigned _col;

public:
    GridPos();
    void setFromXML(QXmlStreamReader &xmlReader, const QString &fileName);
    bool isSet() const;
    unsigned row() const;
    unsigned col() const;
};

#endif // GRID_POS_H
