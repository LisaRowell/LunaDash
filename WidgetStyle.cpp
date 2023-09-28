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

#include "WidgetStyles.h"
#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

const QVector<QString> WidgetStyle::allowedAttrs = { "name", "base" };

WidgetStyle::WidgetStyle() : base(nullptr) {
}

WidgetStyle::WidgetStyle(XMLFileReader &xmlReader,
                         WidgetStyles &widgetStyles) {
    set(xmlReader, widgetStyles);
}

void WidgetStyle::set(XMLFileReader &xmlReader, WidgetStyles &widgetStyles) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

    name_ = stringAttribute("name", xmlReader);

    // Make sure that the user hasn't defined multiple styles with the same
    // name as it's required to be a unique.
    if (widgetStyles.styleExists(name_)) {
        duplicateStyleNamesErrors(name_, xmlReader);
    }

    setBase(xmlReader, widgetStyles);

    while (xmlReader.readNextStartElement()) {
        QStringView elementName = xmlReader.name();
        if (elementName.compare("BackgroundColor") == 0) {
            backgroundColor_ = xmlReader.readElementText();
        } else if (elementName.compare("Color") == 0) {
            color_ = xmlReader.readElementText();
        } else if (elementName.compare("BorderWidth") == 0) {
            borderWidth_ = xmlReader.readElementText();
        } else if (elementName.compare("BorderStyle") == 0) {
            borderStyle_ = xmlReader.readElementText();
        } else if (elementName.compare("BorderRadius") == 0) {
            borderRadius_ = xmlReader.readElementText();
        } else if (elementName.compare("BorderColor") == 0) {
            borderColor_ = xmlReader.readElementText();
        } else if (elementName.compare("Padding") == 0) {
            padding_ = xmlReader.readElementText();
        } else if (elementName.compare("TopPadding") == 0) {
            topPadding_ = xmlReader.readElementText();
        } else if (elementName.compare("BottomPadding") == 0) {
            bottomPadding_ = xmlReader.readElementText();
        } else if (elementName.compare("LeftPadding") == 0) {
            leftPadding_ = xmlReader.readElementText();
        } else if (elementName.compare("RightPadding") == 0) {
            rightPadding_ = xmlReader.readElementText();
        } else {
            unsupportedChildElement("Style", xmlReader);
            xmlReader.skipCurrentElement();
        }
    }

    if (!name_.isEmpty()) {
        widgetStyles.addStyle(this);
    }
}

void WidgetStyle::setBase(const XMLFileReader &xmlReader,
                          const WidgetStyles &widgetStyles) {
    const QString baseName = stringAttribute("base", xmlReader);

    if (!baseName.isEmpty()) {
        if (widgetStyles.styleExists(baseName)) {
            base = widgetStyles.find(baseName);
        } else {
            // Warning goes here
        }
    } else {
        base = nullptr;
    }
}

const QString &WidgetStyle::name() const {
    return name_;
}

QString WidgetStyle::styleSheet() const {
    QString styleSheet;
    QTextStream styleSheetStream(&styleSheet);

    const QString &backgroundColorToUse = backgroundColor();
    if (!backgroundColorToUse.isEmpty()) {
        styleSheetStream << "background-color: " << backgroundColorToUse
                         << ";";
    }
    const QString &colorToUse = color();
    if (!colorToUse.isEmpty()) {
        styleSheetStream << "color: " << colorToUse << ";";
    }
    const QString &borderWidthToUse = borderWidth();
    if (!borderWidthToUse.isEmpty()) {
        styleSheetStream << "border-width: " << borderWidthToUse << ";";
    }
    const QString &borderStyleToUse = borderStyle();
    if (!borderStyleToUse.isEmpty()) {
        styleSheetStream << "border-style: " << borderStyleToUse << ";";
    }
    const QString &borderRadiusToUse = borderRadius();
    if (!borderRadiusToUse.isEmpty()) {
        styleSheetStream << "border-radius: " << borderRadiusToUse << ";";
    }
    const QString &borderColorToUse = borderColor();
    if (!borderColorToUse.isEmpty()) {
        styleSheetStream << "border-color: " << borderColorToUse << ";";
    }
    const QString &paddingToUse = padding();
    if (!paddingToUse.isEmpty()) {
        styleSheetStream << "padding: " << paddingToUse << ";";
    }
    const QString &topPaddingToUse = topPadding();
    if (!topPaddingToUse.isEmpty()) {
        styleSheetStream << "padding-top: " << topPaddingToUse << ";";
    }
    const QString &bottomPaddingToUse = bottomPadding();
    if (!bottomPaddingToUse.isEmpty()) {
        styleSheetStream << "padding-bottom: " << bottomPaddingToUse << ";";
    }
    const QString &leftPaddingToUse = leftPadding();
    if (!leftPaddingToUse.isEmpty()) {
        styleSheetStream << "padding-left: " << leftPaddingToUse << ";";
    }
    const QString &rightPaddingToUse = rightPadding();
    if (!rightPaddingToUse.isEmpty()) {
        styleSheetStream << "padding-right: " << rightPaddingToUse << ";";
    }

    return styleSheet;
}

const QString WidgetStyle::backgroundColor() const {
    if (!backgroundColor_.isEmpty()) {
        return backgroundColor_;
    } else {
        if (base) {
            return base->backgroundColor();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::color() const {
    if (!color_.isEmpty()) {
        return color_;
    } else {
        if (base) {
            return base->color();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::borderWidth() const {
    if (!borderWidth_.isEmpty()) {
        return borderWidth_;
    } else {
        if (base) {
            return base->borderWidth();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::borderStyle() const {
    if (!borderStyle_.isEmpty()) {
        return borderStyle_;
    } else {
        if (base) {
            return base->borderStyle();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::borderRadius() const {
    if (!borderRadius_.isEmpty()) {
        return borderRadius_;
    } else {
        if (base) {
            return base->borderRadius();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::borderColor() const {
    if (!borderColor_.isEmpty()) {
        return borderColor_;
    } else {
        if (base) {
            return base->borderColor();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::padding() const {
    if (!padding_.isEmpty()) {
        return padding_;
    } else {
        if (base) {
            return base->padding();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::topPadding() const {
    if (!topPadding_.isEmpty()) {
        return topPadding_;
    } else {
        if (base) {
            return base->topPadding();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::bottomPadding() const {
    if (!topPadding_.isEmpty()) {
        return topPadding_;
    } else {
        if (base) {
            return base->topPadding();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::leftPadding() const {
    if (!leftPadding_.isEmpty()) {
        return leftPadding_;
    } else {
        if (base) {
            return base->leftPadding();
        } else {
            return "";
        }
    }
}

const QString WidgetStyle::rightPadding() const {
    if (!rightPadding_.isEmpty()) {
        return rightPadding_;
    } else {
        if (base) {
            return base->rightPadding();
        } else {
            return "";
        }
    }
}

[[noreturn]] void
WidgetStyle::duplicateStyleNamesErrors(const QString &name,
                                       const XMLFileReader &xmlReader) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << "Non-unique style name '" << name << "' found in file "
                << xmlReader.fileReference() << ".";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Non-Unique Style Error", errorStr);
    exit(EXIT_FAILURE);
}
