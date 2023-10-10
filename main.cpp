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
#include <QCommandLineParser>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

static QString selectDashboardFile();
[[noreturn]] static void missingXMLDocumentError(const XMLFileReader &xmlReader);
[[noreturn]] static void xmlDocumentTypeError(const XMLFileReader &xmlReader);

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    QCoreApplication::setApplicationName("LunaDash");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("LunaDash MQTT Dashoard");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("dashboard", "Dashboard file to display.");

    parser.process(application);

    const QStringList args = parser.positionalArguments();

    QString configFileName;
    if (args.count() >= 1) {
        configFileName = argv[1];
    } else {
        configFileName = selectDashboardFile();
        if (configFileName.isEmpty()) {
            exit(EXIT_SUCCESS);
        }
    }

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

static QString selectDashboardFile() {
    QFileDialog fileSelector;

    fileSelector.setFileMode(QFileDialog::ExistingFile);
    fileSelector.setNameFilter("Dashboards (*.ldash)");
    fileSelector.setViewMode(QFileDialog::Detail);

    if (fileSelector.exec()) {
        QStringList fileNames;
        fileNames = fileSelector.selectedFiles();
        return fileNames.first();
    } else {
        return "";
    }
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
