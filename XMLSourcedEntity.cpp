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
#include <QStringView>

XMLSourcedEntity::XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                                   const QVector<QString> &requiredAttrs)
    : allowedAttrs(allowedAttrs), requiredAttrs(requiredAttrs) {
}

void XMLSourcedEntity::checkAttrs(const XMLFileReader &xmlReader) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
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

QString XMLSourcedEntity::stringAttribute(const QString &name,
                                          const XMLFileReader &xmlReader,
                                          const QString defaultValue) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        return attribute.toString();
    } else {
        return defaultValue;
    }
}

unsigned XMLSourcedEntity::unsignedAttribute(const QString &name,
                                             const XMLFileReader &xmlReader,
                                             unsigned defaultValue) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        bool valid;
        unsigned value = attribute.toUInt(&valid);
        if (valid) {
            return value;
        } else {
            badUnsignedAttrWarning(name, attribute, xmlReader);
            return defaultValue;
        }
    } else {
        return defaultValue;
    }
}

unsigned XMLSourcedEntity::unsignedAttribute(const QString &name,
                                             const XMLFileReader &xmlReader,
                                             bool *valid) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        unsigned value = attribute.toUInt(valid);
        if (valid) {
            return value;
        } else {
            badUnsignedAttrWarning(name, attribute, xmlReader);
            return 0;
        }
    } else {
        *valid = false;
        return 0;
    }
}

unsigned short
XMLSourcedEntity::ushortAttribute(const QString &name,
                                  const XMLFileReader &xmlReader,
                                  unsigned short defaultValue) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        bool valid;
        unsigned short value = attribute.toUShort(&valid);
        if (valid) {
            return value;
        } else {
            badUShortAttrWarning(name, attribute, xmlReader);
            return defaultValue;
        }
    } else {
        return defaultValue;
    }
}

bool XMLSourcedEntity::boolAttribute(const QString &name,
                                     const XMLFileReader &xmlReader,
                                     bool defaultValue) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        if (attribute.compare("true") == 0) {
            return true;
        } else if (attribute.compare("false") == 0) {
            return false;
        } else {
            badBoolAttrWarning(name, attribute, xmlReader);
            return defaultValue;
        }
    } else {
        return defaultValue;
    }
}

double XMLSourcedEntity::doubleAttribute(const QString &name,
                                         const XMLFileReader &xmlReader,
                                         bool *valid) const {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    QStringView attribute = attributes.value(name);
    if (!attribute.isEmpty()) {
        double value = attribute.toDouble(valid);
        if (valid) {
            return value;
        } else {
            badUnsignedAttrWarning(name, attribute, xmlReader);
            return 0;
        }
    } else {
        *valid = false;
        return 0;
    }
}

QString XMLSourcedEntity::stringElement(const QString &attributeName,
                                        XMLFileReader &xmlReader) {
    verifyLoneAttribute(attributeName, xmlReader);
    QString value = stringAttribute(attributeName, xmlReader);

    QString elementName = xmlReader.name().toString();
    ignoreChildElements(xmlReader, elementName);

    return value;
}

void XMLSourcedEntity::verifyLoneAttribute(const QString &attributeName,
                                           const XMLFileReader &xmlReader) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    bool attributeFound = false;

    for (auto iterator = attributes.begin(); iterator != attributes.end();
         ++iterator) {
        const QXmlStreamAttribute &attribute = *iterator;

        if (attribute.name().compare(attributeName) == 0) {
            attributeFound = true;
        } else {
            unsupportedAttrWarning(attribute, xmlReader);
        }
    }

    if (!attributeFound) {
        missingRequiredAttrError(attributeName, xmlReader);
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

void
XMLSourcedEntity::badUnsignedAttrWarning(const QString &name,
                                         const QStringView &attribute,
                                         const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Bad " << xmlReader.name() << " " << name
                  << " attribute '" << attribute << "' in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Allowed values are unsigned numbers. Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Bad Unsigned Attribute Warning", warningStr);
}

void
XMLSourcedEntity::badUShortAttrWarning(const QString &name,
                                       const QStringView &attribute,
                                       const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Bad " << xmlReader.name() << " " << name
                  << " attribute '" << attribute << "' in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Allowed values are unsigned numbers from 0 to 65,535. Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Bad Unsigned Attribute Warning", warningStr);
}

void
XMLSourcedEntity::badBoolAttrWarning(const QString &name,
                                     const QStringView &attribute,
                                     const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Bad " << xmlReader.name() << " " << name
                  << " attribute '" << attribute << "' in file "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Allowed values are 'true' and 'false'. Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Bad Bool Attribute Warning", warningStr);
}
