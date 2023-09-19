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
#include <QXmlStreamReader>
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
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        fileOpenError(fileName, file);
    }

    QXmlStreamReader xmlReader(&file);
    readConfigStart(xmlReader, fileName);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("MQTTBroker") == 0) {
            new MQTTClient(xmlReader, fileName, variables);
        } else if (elementName.compare("Label") == 0) {
            addLabel(xmlReader, fileName);
        } else if (elementName.compare("Text") == 0) {
            addText(xmlReader, fileName);
        } else {
            unknownElementError(elementName, "Dashboard", fileName,
                                xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
    if (xmlReader.hasError()) {
        xmlParseError(fileName, xmlReader);
    }

    file.close();
}

void Dashboard::readConfigStart(QXmlStreamReader &xmlReader,
                                const QString &fileName) {
    if (!xmlReader.readNext()) {
        xmlParseError(fileName, xmlReader);
    }
    if (!xmlReader.isStartDocument()) {
        xmlDocumentStartError(fileName);
    }
    if (!xmlReader.readNextStartElement()) {
        xmlParseError(fileName, xmlReader);
    }
    if (xmlReader.name().compare("Dashboard") != 0) {
        xmlDocumentTypeError(fileName, xmlReader);
    }
}

void Dashboard::addLabel(QXmlStreamReader &xmlReader, const QString &fileName) {
    Label *label = new Label(xmlReader, fileName);
    addWidgetToLayout(label, label->gridPos(), xmlReader, fileName);
}

void Dashboard::addText(QXmlStreamReader &xmlReader, const QString &fileName) {
    Text *text = new Text(xmlReader, fileName, variables);
    addWidgetToLayout(text, text->gridPos(), xmlReader, fileName);
}

void Dashboard::addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                                  QXmlStreamReader &xmlReader,
                                  const QString &fileName) {
    if (gridPos) {
        layout->addWidget(widget, gridPos->row(), gridPos->col());
    } else {
        missingGridPosWarning("Label", fileName, xmlReader);
        // We're a little sloppy here and leak the widget, but it's no worse
        // memory wise than having a configured one.
    }

}

// We had some error opening the config file. Show an error message and
// abandon ship.
void Dashboard::fileOpenError(const QString &fileName, const QFile &file) const {
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
void Dashboard::xmlParseError(const QString &fileName,
                              const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file '" << fileName << "' ("
                << xmlReader.lineNumber() << "," << xmlReader.columnNumber()
                <<"):" << Qt::endl;
    errorStream << xmlReader.errorString();

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::xmlDocumentStartError(const QString &fileName) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Failed to parse config file '" << fileName << "':" << Qt::endl;
    errorStream << "Missing document start";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Parse Error", errorStr);
    exit(EXIT_FAILURE);
}

// The root XML element wasn't the type we expected.
void Dashboard::xmlDocumentTypeError(const QString &fileName,
                                     const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Incorrect document type '" << xmlReader.name() << "' in '"
                << fileName << "' (" << xmlReader.lineNumber() << ", "
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Expected Dashboard.";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Config Document Type Error", errorStr);
    exit(EXIT_FAILURE);
}

void Dashboard::unknownElementError(const QStringView &name,
                                    const QString &parentName,
                                    const QString &fileName,
                                    const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << parentName <<" contains unsupported element '" << name
                << "' in file '" << fileName << "' ("
                << xmlReader.lineNumber() << ", " << xmlReader.columnNumber()
                << "):" << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Element Error", errorStr);
}

void Dashboard::missingGridPosWarning(const QString &widgetType,
                                      const QString &fileName,
                                      const QXmlStreamReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Dashboard " << widgetType << " widget missing GridPos in '"
                << fileName << "' (" << xmlReader.lineNumber() << ", "
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Missing GridPos Error", errorStr);
}
