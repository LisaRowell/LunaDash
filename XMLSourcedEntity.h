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

#ifndef XML_SOURCED_ENTITY_H
#define XML_SOURCED_ENTITY_H

#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>

class XMLSourcedEntity {
private:
    const QVector<QString> &allowedAttrs;
    const QVector<QString> &requiredAttrs;

    void verifyLoneAttribute(const QString &attributeName,
                             const XMLFileReader &xmlReader);
    void unsupportedAttrWarning(const QXmlStreamAttribute &attribute,
                                const XMLFileReader &xmlReader) const;
    [[noreturn]] void
    missingRequiredAttrError(const QString &attributeName,
                             const XMLFileReader &xmlReader) const;
    void badUnsignedAttrWarning(const QString &name,
                                const QStringView &attribute,
                                const XMLFileReader &xmlReader) const;
    void badUShortAttrWarning(const QString &name,
                              const QStringView &attribute,
                              const XMLFileReader &xmlReader) const;
    void badBoolAttrWarning(const QString &name, const QStringView &attribute,
                            const XMLFileReader &xmlReader) const;

protected:
    void checkAttrs(const XMLFileReader &xmlReader);
    void ignoreChildElements(XMLFileReader &xmlReader,
                             const QString &parentName);
    QString stringAttribute(const QString &name,
                            const XMLFileReader &xmlReader,
                            QString defaultValue = "") const;
    unsigned unsignedAttribute(const QString &name,
                               const XMLFileReader &xmlReader,
                               unsigned defaultValue = 0) const;
    unsigned short ushortAttribute(const QString &name,
                                   const XMLFileReader &xmlReader,
                                   unsigned short defaultValue) const;
    bool boolAttribute(const QString &name, const XMLFileReader &xmlReader,
                       bool defaultValue = false) const;
    QString stringElement(const QString &attributeName,
                          XMLFileReader &xmlReader);
    void unsupportedChildElement(const QString &parentName,
                                 const XMLFileReader &xmlReader) const;

public:
    XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                     const QVector<QString> &requiredAttrs);
};

#endif // XML_SOURCED_ENTITY_H
