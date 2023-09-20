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
#include "LabelWidget.h"
#include "TextWidget.h"
#include "Variables.h"
#include "GridPos.h"

#include <QLayout>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QStringView>

const QVector<QString> Dashboard::allowedAttrs = { };
const QVector<QString> Dashboard::requiredAttrs = { };

Dashboard::Dashboard(XMLFileReader &xmlReader, QWidget *parent)
    : QMainWindow(parent), XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    initWindow();

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("MQTTBroker") == 0) {
            new MQTTClient(xmlReader, variables);
        } else if (elementName.compare("Label") == 0) {
            addLabel(xmlReader);
        } else if (elementName.compare("Text") == 0) {
            addText(xmlReader);
        } else {
            unsupportedChildElement("Dashboard", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

Dashboard::~Dashboard() { 
}

void Dashboard::initWindow() {
    setWindowTitle("Luna");

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    layout = new QGridLayout(mainWidget);
}

void Dashboard::addLabel(XMLFileReader &xmlReader) {
    LabelWidget *labelWidget = new LabelWidget(xmlReader);
    addWidgetToLayout(labelWidget, labelWidget->gridPos(), xmlReader);
}

void Dashboard::addText(XMLFileReader &xmlReader) {
    TextWidget *textWidget = new TextWidget(xmlReader, variables);
    addWidgetToLayout(textWidget, textWidget->gridPos(), xmlReader);
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
