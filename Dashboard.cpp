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

#include "Adder.h"
#include "Concatenator.h"
#include "DurationDescriber.h"
#include "MQTTClient.h"
#include "StringDecoder.h"
#include "Variables.h"
#include "WidgetGrid.h"
#include "WidgetStyle.h"
#include "XMLFileReader.h"

#include <QApplication>
#include <QFile>
#include <QKeySequence>
#include <QMessageBox>
#include <QShortcut>
#include <QString>
#include <QStringView>
#include <QTextStream>
#include <QVector>

const QVector<QString> Dashboard::allowedAttrs = { "title" };
const QString Dashboard::className = "Dashboard";

Dashboard::Dashboard(XMLFileReader &xmlReader, QWidget *parent)
    : QMainWindow(parent) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

    initWindow(xmlReader);
    createShortcuts();

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Style") == 0) {
            new WidgetStyle(xmlReader, widgetStyles);
        } else if (elementName.compare("MQTTBroker") == 0) {
            new MQTTClient(xmlReader, variables);
        } else if (elementName.compare("DurationDescriber") == 0) {
            new DurationDescriber(xmlReader, variables);
        } else if (elementName.compare("StringDecoder") == 0) {
            new StringDecoder(xmlReader, variables);
        } else if (elementName.compare("Adder") == 0) {
            new Adder(xmlReader, variables);
        } else if (elementName.compare("Concatenator") == 0) {
            new Concatenator(xmlReader, variables);
        } else {
            const bool handled
                = layout->handleXMLElement(elementName, xmlReader, variables,
                                           widgetStyles);
            if (!handled) {
                unsupportedChildElement("Dashboard", xmlReader);
                xmlReader.skipCurrentElement();
            }
        }
    }
}

Dashboard::~Dashboard() { 
}

void Dashboard::initWindow(XMLFileReader &xmlReader) {
    const QString title = stringAttribute("title", xmlReader);
    if (!title.isEmpty()) {
        setWindowTitle(title);
    } else {
        setWindowTitle("LunaDash");
    }

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    layout = new WidgetGrid(className, mainWidget);
}

void Dashboard::createShortcuts() {
    QShortcut *exitShortcut
        = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
    connect(exitShortcut, &QShortcut::activated, this, &Dashboard::exit);
}

void Dashboard::exit() {
    QApplication::quit();
}
