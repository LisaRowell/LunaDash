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

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QMessageBox>

XMLSourcedEntity::XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                                   const QVector<QString> &requiredAttrs)
    : allowedAttrs(allowedAttrs), requiredAttrs(requiredAttrs) {
}

void XMLSourcedEntity::checkAttrs(const QXmlStreamAttributes &attributes,
                                  const QString &fileName,
                                  const QXmlStreamReader &xmlReader) {
    QVector<QString> requiredNotFound(requiredAttrs);

    for (auto iterator = attributes.begin(); iterator != attributes.end();
         ++iterator) {
        const QXmlStreamAttribute &attribute = *iterator;

        if (!allowedAttrs.contains(attribute.name())) {
            unsupportedAttrError(attribute, fileName, xmlReader);
        }

        requiredNotFound.removeOne(attribute.name());
    }

    if (requiredNotFound.count()) {
        missingRequiredAttrError(requiredNotFound.first(), fileName, xmlReader);
    }
}

void XMLSourcedEntity::ignoreChildElements(QXmlStreamReader &xmlReader,
                                           const QString &parentName,
                                           const QString &fileName) {
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(parentName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

void
XMLSourcedEntity::unsupportedChildElement(const QString &parentName,
                                          const QString &fileName,
                                          const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Unsupported " << parentName << " child element '"
                << xmlReader.name() << "' found in file '" << fileName
                << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Child Element", errorStr);
}

void
XMLSourcedEntity::unsupportedAttrError(const QXmlStreamAttribute &attribute,
                                       const QString &fileName,
                                       const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Unsupported " << xmlReader.name() << " attribute '"
                << attribute.name() << "' found in file '" << fileName
                << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Attribute", errorStr);
}

void
XMLSourcedEntity::missingRequiredAttrError(const QString &attributeName,
                                           const QString &fileName,
                                           const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Missing " << xmlReader.name() << " required attribute "
                << attributeName << " in file '" << fileName << "' ("
                << xmlReader.lineNumber() << "," << xmlReader.columnNumber()
                << ").";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Missing Attribute", errorStr);
    exit(EXIT_FAILURE);
}
