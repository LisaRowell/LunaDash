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

#include "XMLFileReader.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QXmlStreamReader>

XMLFileReader::XMLFileReader(const QString &fileName)
    : fileName(fileName), file(fileName) {
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        fileOpenError();
    }

    setDevice(&file);

    if (!readNext()) {
        xmlParseError();
    }
    if (!isStartDocument()) {
        xmlDocumentStartError();
    }
}

XMLFileReader::~XMLFileReader() {
    file.close();
}

bool XMLFileReader::readNextStartElement() {
    bool hasNextElement = QXmlStreamReader::readNextStartElement();

    if (hasNextElement) {
        return true;
    } else {
        if (hasError()) {
            xmlParseError();
        } else {
            return false;
        }
    }
}

QString XMLFileReader::fileReference() const {
    QString reference;
    QTextStream referenceStream(&reference);

    referenceStream << "'" << fileName << "' (" << lineNumber() << ", "
                    << columnNumber() << ")";

    return reference;
}

[[noreturn]] void XMLFileReader::fileOpenError() const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Failed to open config file '" << fileName << "':" << Qt::endl;
    errorStream << file.errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config File Open Error", errorStr);
    exit(EXIT_FAILURE);
}

// The Qt XML stream parse found something that it didn't like. Show an error
// message with it's error description and exit.
[[noreturn]] void XMLFileReader::xmlParseError() const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file " << fileReference() << ":"
                << Qt::endl;
    errorStream << errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

[[noreturn]] void XMLFileReader::xmlDocumentStartError() const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Failed to parse config file " << fileReference() << ":"
                << Qt::endl;
    errorStream << "Missing document start";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}
