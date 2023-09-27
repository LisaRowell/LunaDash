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

#include "DurationDescriber.h"

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QString>
#include <QVector>

const QVector<QString> DurationDescriber::allowedAttrs = { "seconds" };
const QVector<QString> DurationDescriber::requiredAttrs = { "seconds" };

DurationDescriber::DurationDescriber(XMLFileReader &xmlReader,
                                     Variables &variables)
    : Transformer(allowedAttrs, requiredAttrs), secondsVariable(nullptr) {
    checkAttrs(xmlReader);
    secondsVariable = findSourceVariable("seconds", "DurationDescriber",
                                         xmlReader, variables);

    parseChildElements(xmlReader, variables);
}

void DurationDescriber::recalculate() {
    if (secondsVariable) {
        bool secondsValid;
        const double seconds = secondsVariable->doubleValue(&secondsValid);
        if (secondsValid) {
            const double minutes = seconds / 60;
            const double hours = minutes / 60;
            const double days = hours / 24;

            QString description;
            if (hours >= 23.95) {
                description = QString::number(days, 'F', 1) + " days";
            } else if (minutes >= 59.95) {
                description = QString::number(minutes, 'F', 1) + " hours";
            } else if (seconds >= 59.95) {
                description = QString::number(minutes, 'F', 1) + " min";
            } else {
                description = QString::number(seconds, 'F', 0) + " sec";
            }

            publishResult(description);
        } else {
            publishResult("---");
        }
    } else {
        // It shouldn't be possible to not have a secondsVariable and still
        // get called to recalculate.
    }
}
