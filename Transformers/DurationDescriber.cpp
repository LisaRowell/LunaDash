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
#include <QTextStream>
#include <QVector>
#include <QMessageBox>

const QVector<QString> DurationDescriber::allowedAttrs = { "seconds", "minutes" };
const QVector<QString> DurationDescriber::requiredAttrs = {};

DurationDescriber::DurationDescriber(XMLFileReader &xmlReader,
                                     Variables &variables) {
    checkAttrs(xmlReader, allowedAttrs, requiredAttrs);
    bool hasSecondsAttr = hasAttribute("seconds", xmlReader);
    bool hasMinutesAttr = hasAttribute("minutes", xmlReader);

    if (hasSecondsAttr && hasMinutesAttr) {
        hasSecondsAndMinutesWarning(xmlReader);
    } else if (!hasSecondsAttr && !hasMinutesAttr) {
        missingSecondsAndMinutesWarning(xmlReader);
    }

    if (hasSecondsAttr && !hasMinutesAttr) {
        secondsVariable = findSourceVariable("seconds", "DurationDescriber",
                                             xmlReader, variables);
        minutesVariable = nullptr;
    } else if (hasMinutesAttr) {
        minutesVariable = findSourceVariable("minutes", "DurationDescriber",
                                             xmlReader, variables);
        secondsVariable = nullptr;
    } else {
        secondsVariable = nullptr;
        minutesVariable = nullptr;
    }

    parseChildElements("DurationDescriber", xmlReader, variables);
}

void DurationDescriber::recalculate() {
    if (secondsVariable) {
        recalculateSeconds();
    } else if (minutesVariable) {
        recalculateMinutes();
    } else {
        // It shouldn't be possible to have neither a secondsVariable nor a
        // minutesVariable and still get called to recalculate.
    }
}

void DurationDescriber::recalculateSeconds() {
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
            description = QString::number(hours, 'F', 1) + " hours";
        } else if (seconds >= 59.95) {
            description = QString::number(minutes, 'F', 1) + " min";
        } else {
            description = QString::number(seconds, 'F', 0) + " sec";
        }

        publishResult(description);
    } else {
        publishResult("---");
    }
}

void DurationDescriber::recalculateMinutes() {
    bool minutesValid;
    const double minutes = minutesVariable->doubleValue(&minutesValid);
    if (minutesValid) {
        const double hours = minutes / 60;
        const double days = hours / 24;

        QString description;
        if (hours >= 23.95) {
            description = QString::number(days, 'F', 1) + " days";
        } else if (minutes >= 59.95) {
            description = QString::number(hours, 'F', 1) + " hours";
        } else {
            description = QString::number(minutes, 'F', 1) + " min";
        }

        publishResult(description);
    } else {
        publishResult("---");
    }
}

void
DurationDescriber::hasSecondsAndMinutesWarning(const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "DurationDescriber with both seconds and minutes attributes in "
                  << "file " << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignoring seconds.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Bad Double Attribute Warning", warningStr);
}

void
DurationDescriber::missingSecondsAndMinutesWarning(const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "DurationDescriber with neither a seconds or a minutes "
                  << "attributes in file " << xmlReader.fileReference() << "."
                  << Qt::endl;
    warningStream << "Ignoring.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Bad Double Attribute Warning", warningStr);
}
