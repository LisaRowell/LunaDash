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

#include "ValuedWidget.h"

#include "Widget.h"
#include "Variables.h"
#include "Variable.h"

#include <QVector>
#include <QString>
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> ValuedWidget::allowedAttrs = { "variable" };
const QVector<QString> ValuedWidget::requiredAttrs = { };

ValuedWidget::ValuedWidget(const QString &widgetType,
                           QXmlStreamReader &xmlReader,
                           const QString &fileName,
                           const Variables &variables)
    : Widget(allowedAttrs, requiredAttrs) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);

    if (attributes.hasAttribute("variable")) {
        const QString &variableName = attributes.value("variable").toString();
        variable = variables.find(variableName);
        if (variable != NULL) {
            connect(variable, SIGNAL(valueChangedSignal()),
                    this, SLOT(valueChanged()));
        } else {
            unknownVariableWarning(widgetType, variableName, xmlReader,
                                   fileName);
        }
    } else {
        variable = NULL;
    }
}

void ValuedWidget::valueChanged() {
    setValue();
}

void ValuedWidget::unknownVariableWarning(const QString &widgetType,
                                          const QString &variableName,
                                          const QXmlStreamReader &xmlReader,
                                          const QString &fileName) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << widgetType <<" with an unknown variable '" << variableName
                << "' in file '" << fileName << "' ("
                << xmlReader.lineNumber() << ", " << xmlReader.columnNumber()
                << "):" << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", errorStr);
}
