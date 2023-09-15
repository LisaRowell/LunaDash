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

#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> Variable::allowedAttrs = { "name" };
const QVector<QString> Variable::requiredAttrs = { "name" };

Variable::Variable(const QString &typeName, QXmlStreamReader &xmlReader,
                   const QString &fileName, Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    name_ = attributes.value("name").toString();

    // Make sure that the user hasn't defined multiple variables with the same
    // name as it's required to be a unique
    if (variables.variableExists(name_)) {
        duplicateVariableNamesErrors(name_, xmlReader, fileName);
    }

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(typeName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

const QString &Variable::name() const {
    return name_;
}

void Variable::valueChanged() {
    emit valueChangedSignal();
}

const QString &Variable::string() const {
    static QString emptyString("");
    return emptyString;
}

void Variable::duplicateVariableNamesErrors(const QString &name,
                                            const QXmlStreamReader &xmlReader,
                                            const QString &fileName) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Non-unique variable name '" << name << "' found in file '"
                << fileName << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ").";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Non-Unique Variable Error", errorStr);
    exit(EXIT_FAILURE);
}
