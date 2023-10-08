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

#include "ScaleThresholds.h"

#include "ScaleThreshold.h"
#include "XMLFileReader.h"

#include <QColor>
#include <QString>
#include <QVector>

ScaleThresholds::ScaleThresholds() {
    overColor_ = QColor("black");
}

void ScaleThresholds::set(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader, emptyAttrsList, emptyAttrsList);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("UpTo") == 0) {
            addThreshold(xmlReader);
        } else if (elementName.compare("Over") == 0) {
            setOverColor(xmlReader);
        } else {
            unsupportedChildElement("Threshold", xmlReader);
        }
    }
}

void ScaleThresholds::addThreshold(XMLFileReader &xmlReader) {
    ScaleThreshold *threshold = new ScaleThreshold(xmlReader);

    QVector<ScaleThreshold *>::const_iterator it;
    for(it = thresholds.constBegin(); it != thresholds.constEnd(); ++it) {
        if (threshold->isBefore(**it)) {
            thresholds.insert(it, threshold);
            return;
        }
    }

    thresholds.append(threshold);
}

void ScaleThresholds::setOverColor(XMLFileReader &xmlReader) {
    QString colorName = stringElement("color", xmlReader);
    overColor_ = QColor(colorName);
}

bool ScaleThresholds::configured() const {
    return !thresholds.isEmpty();
}

const QColor &ScaleThresholds::colorForValue(double value) const {
    foreach (ScaleThreshold *threshold, thresholds) {
        if (threshold->valueInRange(value)) {
            return threshold->color();
        }
    }

    return overColor_;
}

unsigned ScaleThresholds::count() const {
    return thresholds.count();
}

const ScaleThreshold *ScaleThresholds::threshold(unsigned index) const {
    return thresholds[index];
}

const QColor &ScaleThresholds::overColor() const {
    return overColor_;
}
