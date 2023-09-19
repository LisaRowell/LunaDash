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

#include "Dashboard.h"
#include "XMLFileReader.h"

#include <QApplication>
#include <QString>
#include <QTextStream>
#include <QMessageBox>

[[noreturn]] static void missingXMLDocumentError(const XMLFileReader &xmlReader);
[[noreturn]] static void xmlDocumentTypeError(const XMLFileReader &xmlReader);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // For now we default to loading the file 'Dashboard.ldash' in the working
    // directory, but at some point this should change to a filename coming
    // from arguements and the user should be able to double click a .ldash
    // file to start running with that dashboard.
    const QString configFileName = "Dashboard.ldash";

    XMLFileReader xmlReader(configFileName);
    if (!xmlReader.readNextStartElement()) {
        missingXMLDocumentError(xmlReader);
    }
    if (xmlReader.name().compare("Dashboard") != 0) {
        xmlDocumentTypeError(xmlReader);
    }

    Dashboard dashboard(xmlReader);
    dashboard.show();

    return application.exec();
}

[[noreturn]] static void missingXMLDocumentError(const XMLFileReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Missing XML document in " << xmlReader.fileReference()
                << "." << Qt::endl;

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}

// The root XML element wasn't the type we expected.
[[noreturn]] static void xmlDocumentTypeError(const XMLFileReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Incorrect document type '" << xmlReader.name() << "' in "
                << xmlReader.fileReference() << "." << Qt::endl;
    errorStream << "Expected Dashboard.";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}
