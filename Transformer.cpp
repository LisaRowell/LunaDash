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

#include "Transformer.h"

#include "DoubleVariable.h"
#include "StringVariable.h"
#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

Transformer::Transformer() {
}

Variable *Transformer::findSourceVariable(const QString &attributeName,
                                          const QString transformerName,
                                          const XMLFileReader &xmlReader,
                                          const Variables &variables) {
    const QString variableName = stringAttribute(attributeName, xmlReader);
    if (!variableName.isEmpty()) {
        Variable *variable = variables.find(variableName);
        if (variable != nullptr) {
            connect(variable, &Variable::valueChangedSignal,
                    this, &Transformer::inputChanged);
        }
        return variable;
    } else {
        unknownVariableWarning(transformerName, variableName, xmlReader);
        return nullptr;
    }
}

void Transformer::parseChildElements(const QString transformerName,
                                     XMLFileReader &xmlReader,
                                     Variables &variables) {
    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        handleChildElement(elementName, transformerName, xmlReader, variables);
    }
}
void Transformer::handleChildElement(const QStringView &elementName,
                                     const QString transformerName,
                                     XMLFileReader &xmlReader,
                                     Variables &variables) {
    if (elementName.compare("String") == 0) {
        addStringVariable(xmlReader, variables);
    } else if (elementName.compare("Double") == 0) {
        addDoubleVariable(xmlReader, variables);
    } else {
        unsupportedChildElement(transformerName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

void Transformer::addStringVariable(XMLFileReader &xmlReader,
                                    Variables &variables) {
    StringVariable *variable = new StringVariable(xmlReader, variables);
    variables.addVariable(variable);
    connect(this, &Transformer::newStringValueSignal,
            variable, &StringVariable::newStringValue);
    connect(this, &Transformer::newDoubleValueSignal,
            variable, &StringVariable::newDoubleValue);
}

void Transformer::addDoubleVariable(XMLFileReader &xmlReader,
                                    Variables &variables) {
    DoubleVariable *variable = new DoubleVariable(xmlReader, variables);
    variables.addVariable(variable);
    connect(this, &Transformer::newStringValueSignal,
            variable, &DoubleVariable::newStringValue);
    connect(this, &Transformer::newDoubleValueSignal,
            variable, &DoubleVariable::newDoubleValue);
}

void Transformer::inputChanged() {
    recalculate();
}

void Transformer::publishResult(const QString &result) {
    emit newStringValueSignal(result);
}

void Transformer::publishResult(double result) {
    emit newDoubleValueSignal(result);
}

void
Transformer::unknownVariableWarning(const QString &transformerName,
                                    const QString &variableName,
                                    const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << transformerName << " with an unknown variable '"
                  << variableName << "' in file " << xmlReader.fileReference()
                  << ":" << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", warningStr);
}
