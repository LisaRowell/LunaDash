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

#include "XMLSourcedEntity.h"
#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QTextStream>
#include <QMessageBox>

XMLSourcedEntity::XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                                   const QVector<QString> &requiredAttrs)
    : allowedAttrs(allowedAttrs), requiredAttrs(requiredAttrs) {
}

void XMLSourcedEntity::checkAttrs(const QXmlStreamAttributes &attributes,
                                  const XMLFileReader &xmlReader) {
    QVector<QString> requiredNotFound(requiredAttrs);

    for (auto iterator = attributes.begin(); iterator != attributes.end();
         ++iterator) {
        const QXmlStreamAttribute &attribute = *iterator;

        if (!allowedAttrs.contains(attribute.name())) {
            unsupportedAttrWarning(attribute, xmlReader);
        }

        requiredNotFound.removeOne(attribute.name());
    }

    if (requiredNotFound.count()) {
        missingRequiredAttrError(requiredNotFound.first(), xmlReader);
    }
}

void XMLSourcedEntity::ignoreChildElements(XMLFileReader &xmlReader,
                                           const QString &parentName) {
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(parentName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

void
XMLSourcedEntity::unsupportedChildElement(const QString &parentName,
                                          const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Unsupported " << parentName << " child element '"
                  << xmlReader.name() << "' found in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Child Element", warningStr);
}

void
XMLSourcedEntity::unsupportedAttrWarning(const QXmlStreamAttribute &attribute,
                                         const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);
    warningStream << "Unsupported " << xmlReader.name() << " attribute '"
                  << attribute.name() << "' found in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Attribute", warningStr);
}

[[noreturn]] void
XMLSourcedEntity::missingRequiredAttrError(const QString &attributeName,
                                           const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Missing " << xmlReader.name() << " required attribute "
                << attributeName << " in file " << xmlReader.fileReference()
                << ".";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Missing Attribute", errorStr);
    exit(EXIT_FAILURE);
}
