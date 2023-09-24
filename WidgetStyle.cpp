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

#include "WidgetStyle.h"

#include "XMLSourcedEntity.h"
#include "XMLFileReader.h"

#include <QVector>
#include <QString>
#include <QTextStream>

const QVector<QString> WidgetStyle::allowedAttrs = { };
const QVector<QString> WidgetStyle::requiredAttrs = { };

WidgetStyle::WidgetStyle()
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
}

void WidgetStyle::set(XMLFileReader &xmlReader) {
    checkAttrs(xmlReader);

    while (xmlReader.readNextStartElement()) {
        QStringView elementName = xmlReader.name();
        if (elementName.compare("BackgroundColor") == 0) {
            backgroundColor = xmlReader.readElementText();
        } else if (elementName.compare("Color") == 0) {
            color = xmlReader.readElementText();
        } else if (elementName.compare("BorderWidth") == 0) {
            borderWidth = xmlReader.readElementText();
        } else if (elementName.compare("BorderStyle") == 0) {
            borderStyle = xmlReader.readElementText();
        } else if (elementName.compare("BorderRadius") == 0) {
            borderRadius = xmlReader.readElementText();
        } else if (elementName.compare("BorderColor") == 0) {
            borderColor = xmlReader.readElementText();
        } else if (elementName.compare("Padding") == 0) {
            padding = xmlReader.readElementText();
        } else if (elementName.compare("TopPadding") == 0) {
            topPadding = xmlReader.readElementText();
        } else if (elementName.compare("BottomPadding") == 0) {
            bottomPadding = xmlReader.readElementText();
        } else if (elementName.compare("LeftPadding") == 0) {
            leftPadding = xmlReader.readElementText();
        } else if (elementName.compare("RightPadding") == 0) {
            rightPadding = xmlReader.readElementText();
        } else {
            unsupportedChildElement("Style", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }
}

QString WidgetStyle::styleSheet() const {
    QString styleSheet;
    QTextStream styleSheetStream(&styleSheet);

    if (!backgroundColor.isEmpty()) {
        styleSheetStream << "background-color: " << backgroundColor << ";";
    }
    if (!color.isEmpty()) {
        styleSheetStream << "color: " << color << ";";
    }
    if (!borderWidth.isEmpty()) {
        styleSheetStream << "border-width: " << borderWidth << ";";
    }
    if (!borderStyle.isEmpty()) {
        styleSheetStream << "border-style: " << borderStyle << ";";
    }
    if (!borderRadius.isEmpty()) {
        styleSheetStream << "border-radius: " << borderRadius << ";";
    }
    if (!borderColor.isEmpty()) {
        styleSheetStream << "border-color: " << borderColor << ";";
    }
    if (!padding.isEmpty()) {
        styleSheetStream << "padding: " << padding << ";";
    }
    if (!topPadding.isEmpty()) {
        styleSheetStream << "padding-top: " << topPadding << ";";
    }
    if (!bottomPadding.isEmpty()) {
        styleSheetStream << "padding-bottom: " << bottomPadding << ";";
    }
    if (!leftPadding.isEmpty()) {
        styleSheetStream << "padding-left: " << leftPadding << ";";
    }
    if (!rightPadding.isEmpty()) {
        styleSheetStream << "padding-right: " << rightPadding << ";";
    }

    return styleSheet;
}
