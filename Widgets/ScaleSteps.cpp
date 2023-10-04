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

#include "ScaleSteps.h"

#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

const QVector<QString> ScaleSteps::allowedAttrs = { "major", "minor" };

ScaleSteps::ScaleSteps()
    : major_(defaultMajorSteps), minor_(defaultMinorSteps) {
}

void ScaleSteps::set(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

    major_ = unsignedAttribute("major", xmlReader, defaultMajorSteps);
    minor_ = unsignedAttribute("minor", xmlReader, defaultMinorSteps);

    ignoreChildElements(xmlReader, "ScaleSteps");
}

unsigned ScaleSteps::major() const {
    return major_;
}

unsigned ScaleSteps::minor() const {
    return minor_;
}
