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

#ifndef XML_FILE_READER_H
#define XML_FILE_READER_H

#include <QXmlStreamReader>
#include <QString>
#include <QFile>

class XMLFileReader : public QXmlStreamReader {
private:
    QString fileName;
    QFile file;

    [[noreturn]] void fileOpenError() const;

public:
    XMLFileReader(const QString &fileName);
    ~XMLFileReader();
    QString fileReference() const;
};

#endif // XML_FILE_READER_H
