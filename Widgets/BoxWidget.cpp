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

#include "BoxWidget.h"

#include "Variable.h"
#include "Variables.h"
#include "WidgetGrid.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"

#include <QMessageBox>
#include <QString>
#include <QStringView>
#include <QTextStream>
#include <QWidget>
#include <QVector>

const QVector<QString> BoxWidget::allowedAttrs = {
    "label", "labelVariable", "flat"
};
const QString BoxWidget::className = "Box";

BoxWidget::BoxWidget(XMLFileReader &xmlReader, const Variables &variables,
                     WidgetStyles &widgetStyles) : labelVariable(nullptr) {
    checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

    BoxWidget::initTitle(xmlReader, variables);

    const bool flat = boolAttribute("flat", xmlReader, false);
    if (flat) {
        setFlat(true);
        setStyleSheet("QGroupBox {border:0;}");
        expandable_ = false;
    } else {
        expandable_ = true;
    }

    layout = new WidgetGrid(className);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        const bool handled = layout->handleXMLElement(elementName, xmlReader,
                                                      variables,
                                                      widgetStyles);
        if (!handled) {
            handleChildElement(xmlReader, "Box");
        }
    }

    setLayout(layout);
}

void BoxWidget::initTitle(const XMLFileReader &xmlReader,
                          const Variables &variables) {
    const QString labelText = stringAttribute("label", xmlReader);
    const QString labelVariableName = stringAttribute("labelVariable", xmlReader);
    if (!labelText.isEmpty()) {
        setTitle(labelText);
        if (!labelVariableName.isEmpty()) {
            labelAndLabelVariableWarning(xmlReader);
        }
    } else if (!labelVariableName.isEmpty()) {
        labelVariable = variables.find(labelVariableName);
        if (labelVariable != nullptr) {
            connect(labelVariable, &Variable::valueChangedSignal,
                    this, &BoxWidget::updateTitle);
            updateTitle();
        } else {
            unknownLabelVariableWarning(labelVariableName, xmlReader);
            setTitle("");
        }
    } else {
        setTitle("");
    }
}

bool BoxWidget::expandable() const {
    return expandable_;
}

void BoxWidget::updateTitle() {
    if (labelVariable != nullptr && labelVariable->hasValue()) {
        setTitle(labelVariable->string());
    } else {
        setTitle("");
    }
}

void
BoxWidget::labelAndLabelVariableWarning(const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Box with both a 'label' and a 'labelVariable' in file "
                  << xmlReader.fileReference() << ":" << Qt::endl;
    warningStream << "Ignored variable.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Label and Label Variable Warning", warningStr);
}

void
BoxWidget::unknownLabelVariableWarning(const QString &variableName,
                                       const XMLFileReader &xmlReader) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Box with an unknown label variable '" << variableName
                  << "' in file " << xmlReader.fileReference() << ":"
                  << Qt::endl;
    warningStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", warningStr);
}
