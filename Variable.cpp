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

#include "Variable.h"

#include "Variables.h"
#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> Variable::allowedAttrs = { "name" };
const QVector<QString> Variable::requiredAttrs = { "name" };

Variable::Variable(const QString &typeName, XMLFileReader &xmlReader,
                   Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), hasValue_(false) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, xmlReader);
    name_ = attributes.value("name").toString();

    // Make sure that the user hasn't defined multiple variables with the same
    // name as it's required to be a unique
    if (variables.variableExists(name_)) {
        duplicateVariableNamesErrors(name_, xmlReader);
    }

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(typeName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

const QString &Variable::name() const {
    return name_;
}

bool Variable::hasValue() const {
    return hasValue_;
}

void Variable::valueChanged() {
    emit valueChangedSignal();
}

[[noreturn]] void
Variable::duplicateVariableNamesErrors(const QString &name,
                                       const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Non-unique variable name '" << name << "' found in file "
                << xmlReader.fileReference() << ".";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Non-Unique Variable Error", errorStr);
    exit(EXIT_FAILURE);
}
