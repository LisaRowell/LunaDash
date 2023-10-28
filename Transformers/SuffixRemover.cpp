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

#include "SuffixRemover.h"

#include "Transformer.h"
#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QString>
#include <QVector>

const QVector<QString> SuffixRemover::allowedAttrs = { "string", "suffix" };
const QVector<QString> SuffixRemover::requiredAttrs = { "string" };

SuffixRemover::SuffixRemover(XMLFileReader &xmlReader, Variables &variables) {
    checkAttrs(xmlReader, allowedAttrs, requiredAttrs);
    inputVariable = findSourceVariable("string", "SuffixRemover", xmlReader,
                                       variables);
    suffix = stringAttribute("suffix", xmlReader);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        handleChildElement(elementName, "SuffixRemover", xmlReader, variables);
    }
}

void SuffixRemover::recalculate() {
    if (inputVariable) {
        QString string = inputVariable->string();
        if (string.endsWith(suffix)) {
            string.chop(suffix.length());
        }
        publishResult(string);
    } else {
        // It shouldn't be possible to not have an inputVariable and still
        // get called to recalculate.
    }
}
