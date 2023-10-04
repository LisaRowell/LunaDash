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

#include "WidgetGrid.h"

#include "BoolWidget.h"
#include "BoxWidget.h"
#include "DialGaugeWidget.h"
#include "GridPos.h"
#include "LabelIndicatorWidget.h"
#include "LabelWidget.h"
#include "NumberWidget.h"
#include "NumericGaugeWidget.h"
#include "SpacerWidget.h"
#include "TabBarWidget.h"
#include "TextWidget.h"
#include "Variables.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QStringView>
#include <QTextStream>

WidgetGrid::WidgetGrid(const QString &containerName, QWidget *parent)
    : QGridLayout(parent), containerName(containerName) {
}

bool WidgetGrid::handleXMLElement(const QStringView &name,
                                  XMLFileReader &xmlReader,
                                  const Variables &variables,
                                  WidgetStyles &widgetStyles) {
    if (name.compare("Label") == 0) {
        addLabelWidget(xmlReader);
        return true;
    } else if (name.compare("Text") == 0) {
        addTextWidget(xmlReader, variables);
        return true;
    } else if (name.compare("Number") == 0) {
        addNumberWidget(xmlReader, variables);
        return true;
    } else if (name.compare("Bool") == 0) {
        addBoolWidget(xmlReader, variables);
        return true;
    } else if (name.compare("LabelIndicator") == 0) {
        addLabelIndicatorWidget(xmlReader, variables, widgetStyles);
        return true;
    } else if (name.compare("NumericGauge") == 0) {
        addNumericGaugeWidget(xmlReader, variables);
        return true;
    } else if (name.compare("DialGauge") == 0) {
        addDialGaugeWidget(xmlReader, variables);
        return true;
    } else if (name.compare("Box") == 0) {
        addBoxWidget(xmlReader, variables, widgetStyles);
        return true;
    } else if (name.compare("TabBar") == 0) {
        addTabBarWidget(xmlReader, variables, widgetStyles);
        return true;
    } else if (name.compare("Spacer") == 0) {
        addSpacerWidget(xmlReader);
        return true;
    } else {
        return false;
    }
}

void WidgetGrid::addLabelWidget(XMLFileReader &xmlReader) {
    LabelWidget *labelWidget = new LabelWidget(xmlReader);
    addWidgetToLayout(labelWidget, labelWidget->gridPos(), "Label",
                      xmlReader);
}

void WidgetGrid::addTextWidget(XMLFileReader &xmlReader,
                               const Variables &variables) {
    TextWidget *textWidget = new TextWidget(xmlReader, variables);
    addWidgetToLayout(textWidget, textWidget->gridPos(), "Text", xmlReader);
}

void WidgetGrid::addNumberWidget(XMLFileReader &xmlReader,
                                 const Variables &variables) {
    NumberWidget *numberWidget = new NumberWidget(xmlReader, variables);
    addWidgetToLayout(numberWidget, numberWidget->gridPos(), "Number",
                      xmlReader);
}

void WidgetGrid::addBoolWidget(XMLFileReader &xmlReader,
                               const Variables &variables) {
    BoolWidget *boolWidget = new BoolWidget(xmlReader, variables);
    addWidgetToLayout(boolWidget, boolWidget->gridPos(), "Bool", xmlReader);
}

void WidgetGrid::addLabelIndicatorWidget(XMLFileReader &xmlReader,
                                         const Variables &variables,
                                         WidgetStyles &widgetStyles) {
    LabelIndicatorWidget *labelIndicatorWidget
        = new LabelIndicatorWidget(xmlReader, variables, widgetStyles);
    addWidgetToLayout(labelIndicatorWidget, labelIndicatorWidget->gridPos(),
                      "LabelIndicator", xmlReader);
}

void WidgetGrid::addNumericGaugeWidget(XMLFileReader &xmlReader,
                                       const Variables &variables) {
    NumericGaugeWidget *numericGaugeWidget
        = new NumericGaugeWidget(xmlReader, variables);
    addWidgetToLayout(numericGaugeWidget, numericGaugeWidget->gridPos(),
                      "Gauge", xmlReader);
}

void WidgetGrid::addDialGaugeWidget(XMLFileReader &xmlReader,
                                    const Variables &variables) {
    DialGaugeWidget *dialGaugeWidget
        = new DialGaugeWidget(xmlReader, variables);
    addWidgetToLayout(dialGaugeWidget, dialGaugeWidget->gridPos(),
                      "DialGauge", xmlReader);
}

void WidgetGrid::addBoxWidget(XMLFileReader &xmlReader,
                              const Variables &variables,
                              WidgetStyles &widgetStyles) {
    BoxWidget *boxWidget
        = new BoxWidget(xmlReader, variables, widgetStyles);
    addWidgetToLayout(boxWidget, boxWidget->gridPos(), "Box", xmlReader,
                      boxWidget->expandable());
}

void WidgetGrid::addTabBarWidget(XMLFileReader &xmlReader,
                                 const Variables &variables,
                                 WidgetStyles &widgetStyles) {
    TabBarWidget *tabBarWidget
        = new TabBarWidget(xmlReader, variables, widgetStyles);
    addWidgetToLayout(tabBarWidget, tabBarWidget->gridPos(), "TabBar",
                      xmlReader, true);
}

void WidgetGrid::addSpacerWidget(XMLFileReader &xmlReader) {
    SpacerWidget *spacerWidget
        = new SpacerWidget(xmlReader);
    const GridPos *gridPos = spacerWidget->gridPos();
    if (gridPos) {
        addItem(spacerWidget, gridPos->row(), gridPos->col(), gridPos->rowSpan(),
                gridPos->colSpan());
    } else {
        missingGridPosWarning("Spacer", xmlReader);
        // We're a little sloppy here and leak the widget, but it's no worse
        // memory wise than having a configured one.
    }
}

void WidgetGrid::addWidgetToLayout(QWidget *widget, const GridPos *gridPos,
                                   const QString &typeName,
                                   XMLFileReader &xmlReader,
                                   bool allowStretch) {
    Qt::Alignment alignment;
    if (allowStretch) {
        alignment = Qt::Alignment();
    } else {
        alignment = Qt::AlignTop | Qt::AlignLeft;
    }

    if (gridPos) {
        addWidget(widget, gridPos->row(), gridPos->col(), gridPos->rowSpan(),
                  gridPos->colSpan(), alignment);
    } else {
        missingGridPosWarning(typeName, xmlReader);
        // We're a little sloppy here and leak the widget, but it's no worse
        // memory wise than having a configured one.
    }
}

void WidgetGrid::missingGridPosWarning(const QString &widgetType,
                                       const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << containerName << " " << widgetType
                  << " widget missing GridPos in "
                  << xmlReader.fileReference() << "." << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Missing GridPos", warningStr);
}
