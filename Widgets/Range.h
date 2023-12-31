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

#ifndef RANGE_H
#define RANGE_H

#include "XMLSourcedEntity.h"

#include <QString>
#include <QVector>

class XMLFileReader;

class Range : XMLSourcedEntity {
private:
    double min_;
    double max_;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

public:
    Range(double min = 0, double max = 100);
    void set(XMLFileReader &xmlReader);
    double min() const;
    double max() const;
};

#endif // RANGE_H
