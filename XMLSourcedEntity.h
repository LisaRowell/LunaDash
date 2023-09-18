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

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QObject>

class XMLSourcedEntity : public QObject {
    Q_OBJECT

private:
    const QVector<QString> &allowedAttrs;
    const QVector<QString> &requiredAttrs;

    void unsupportedAttrError(const QXmlStreamAttribute &attribute,
                              const QString &fileName,
                              const QXmlStreamReader &xmlReader) const;
    void missingRequiredAttrError(const QString &attributeName,
                                  const QString &fileName,
                                  const QXmlStreamReader &xmlReader) const;

protected:
    void checkAttrs(const QXmlStreamAttributes &attributes,
                    const QString &fileName,
                    const QXmlStreamReader &xmlReader);
    void ignoreChildElements(QXmlStreamReader &xmlReader,
                             const QString &parentName,
                             const QString &fileName);
    void unsupportedChildElement(const QString &parentName,
                                 const QString &fileName,
                                 const QXmlStreamReader &xmlReader) const;

public:
    XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                     const QVector<QString> &requiredAttrs);
};

#endif // XML_SOURCED_ENTITY_H