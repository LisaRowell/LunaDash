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
#include "MQTTClient.h"
#include "Label.h"
#include "Text.h"
#include "Variables.h"
#include "GridPos.h"

#include <QLayout>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QStringView>

Dashboard::Dashboard(const QString &configFileName, QWidget *parent)
    : QMainWindow(parent) {
    initWindow();
    readConfig(configFileName, variables);
}

Dashboard::~Dashboard() { 
}

void Dashboard::initWindow() {
    setWindowTitle("Luna");

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    layout = new QGridLayout(mainWidget);
}

// Pull in the XML the configuration file, doing a streaming read, creating
// objects as they're found.
void Dashboard::readConfig(const QString &fileName, Variables &variables) {
    XMLFileReader xmlReader(fileName);

    readConfigStart(xmlReader);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("MQTTBroker") == 0) {
            new MQTTClient(xmlReader, variables);
        } else if (elementName.compare("Label") == 0) {
            addLabel(xmlReader);
        } else if (elementName.compare("Text") == 0) {
            addText(xmlReader);
        } else {
            unknownElementWarning("Dashboard", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
    if (xmlReader.hasError()) {
        xmlParseError(xmlReader);
    }
}

void Dashboard::readConfigStart(XMLFileReader &xmlReader) {
    if (!xmlReader.readNext()) {
        xmlParseError(xmlReader);
    }
    if (!xmlReader.isStartDocument()) {
        xmlDocumentStartError(xmlReader);
    }
    if (!xmlReader.readNextStartElement()) {
        xmlParseError(xmlReader);
    }
    if (xmlReader.name().compare("Dashboard") != 0) {
        xmlDocumentTypeError(xmlReader);
    }
}

void Dashboard::addLabel(XMLFileReader &xmlReader) {
    Label *label = new Label(xmlReader);
    addWidgetToLayout(label, label->gridPos(), xmlReader);
}

void Dashboard::addText(XMLFileReader &xmlReader) {
    Text *text = new Text(xmlReader, variables);
    addWidgetToLayout(text, text->gridPos(), xmlReader);
}

void Dashboard::addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                                  XMLFileReader &xmlReader) {
    if (gridPos) {
        layout->addWidget(widget, gridPos->row(), gridPos->col());
    } else {
        missingGridPosWarning("Label", xmlReader);
        // We're a little sloppy here and leak the widget, but it's no worse
        // memory wise than having a configured one.
    }
}

// The Qt XML stream parse found something that it didn't like. Show an error
// message with it's error description and exit.
[[noreturn]] void
Dashboard::xmlParseError(const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file " << xmlReader.fileReference()
                <<":" << Qt::endl;
    errorStream << xmlReader.errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

[[noreturn]] void
Dashboard::xmlDocumentStartError(const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Failed to parse config file " << xmlReader.fileReference()
                << ":" << Qt::endl;
    errorStream << "Missing document start";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

// The root XML element wasn't the type we expected.
[[noreturn]] void
Dashboard::xmlDocumentTypeError(const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Incorrect document type '" << xmlReader.name() << "' in "
                << xmlReader.fileReference() << "." << Qt::endl;
    errorStream << "Expected Dashboard.";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::unknownElementWarning(const QString &parentName,
                                      const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << parentName <<" contains unsupported element '"
                  << xmlReader.name() << "' in file "
                  << xmlReader.fileReference() << ":" << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Element", warningStr);
}

void Dashboard::missingGridPosWarning(const QString &widgetType,
                                      const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);
    warningStream << "Dashboard " << widgetType
                  << " widget missing GridPos in "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Missing GridPos", warningStr);
}
