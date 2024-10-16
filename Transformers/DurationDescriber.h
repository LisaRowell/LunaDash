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

#ifndef DURATION_DESCRIBER_H
#define DURATION_DESCRIBER_H

#include "Transformer.h"

#include <QString>
#include <QVector>

class Variable;
class Variables;
class XMLFileReader;

class DurationDescriber : public Transformer {
private:
    Variable *secondsVariable;
    Variable *minutesVariable;

    virtual void recalculate() override;
    void recalculateSeconds();
    void recalculateMinutes();

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    void hasSecondsAndMinutesWarning(const XMLFileReader &xmlReader) const;
    void missingSecondsAndMinutesWarning(const XMLFileReader &xmlReader) const;

public:
    DurationDescriber(XMLFileReader &xmlReader, Variables &variables);
};

#endif // DURATION_DESCRIBER_H
