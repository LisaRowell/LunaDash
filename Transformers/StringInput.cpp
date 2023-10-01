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

#include "StringInput.h"

#include "Transformer.h"
#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

const QVector<QString> StringInput::allowedAttrs = { "text", "variable" };

StringInput::StringInput(XMLFileReader &xmlReader, Variables &variables,
                         const QString &transformerName) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

    bool textPresent = hasAttribute("text", xmlReader);
    bool variablePresent = hasAttribute("variable", xmlReader);

    if ((!textPresent && !variablePresent) ||
        (textPresent && variablePresent)) {
        attributeWarning(transformerName, xmlReader);
        isConstant = true;
        input.string = new QString();
    } else {
        if (textPresent) {
            isConstant = true;
            input.string = new QString(stringAttribute("text", xmlReader));
        } else {
            const QString &variableName = stringAttribute("variable",
                                                          xmlReader);
            input.variable = variables.find(variableName);
            if (input.variable != nullptr) {
                connect(input.variable, &Variable::valueChangedSignal,
                        this, &StringInput::variableChanged);
            } else {
                unknownVariableWarning(variableName, xmlReader,
                                       transformerName);
            }
        }
    }

    ignoreChildElements(xmlReader, "Input");
}

void StringInput::variableChanged() {
    emit inputChangedSignal();
}

const QString StringInput::value() const {
    if (isConstant) {
        return *input.string;
    } else {
        return input.variable->string();
    }
}

void StringInput::attributeWarning(const QString &transformerName,
                                   const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << transformerName << " with a bad string input in file "
                  << xmlReader.fileReference() << Qt::endl;
    warningStream << "String inputs require either a 'text' attribute or a "
                  << "'variable' attribute, but not both." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", warningStr);
}


void
StringInput::unknownVariableWarning(const QString &variableName,
                                    const XMLFileReader &xmlReader,
                                    const QString &transformerName) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << transformerName << " with an unknown input variable '"
                  << variableName << "' in file " << xmlReader.fileReference()
                  << ":" << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", warningStr);
}
