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

#include "LabelIndicatorWidget.h"

#include "ValuedWidget.h"
#include "Variable.h"
#include "Variables.h"
#include "WidgetStyles.h"
#include "XMLFileReader.h"

#include <QString>
#include <QVector>

const QVector<QString> LabelIndicatorWidget::additionalAllowedAttrs = {
    "text"
};

LabelIndicatorWidget::LabelIndicatorWidget(XMLFileReader &xmlReader,
                                           const Variables &variables,
                                           WidgetStyles &widgetStyles)
    : ValuedWidget("LabelIndicator", xmlReader, variables,
                   additionalAllowedAttrs, emptyAttrsList) {
    const QString label = stringAttribute("text", xmlReader);
    setText(label);

    while (xmlReader.readNextStartElement()) {
        QStringView elementName = xmlReader.name();
        if (elementName.compare("OnStyle") == 0) {
            onStyle.set(xmlReader, widgetStyles);
        } else if (elementName.compare("OffStyle") == 0) {
            offStyle.set(xmlReader, widgetStyles);
        } else {
            handleChildElement(xmlReader, "Label");
        }
    }

    LabelIndicatorWidget::setValue();
}

void LabelIndicatorWidget::setValue() {
    if (variable != NULL && variable->hasValue()) {
        bool valid;
        double value = variable->boolValue(&valid);
        if (valid) {
            if (value) {
                setStyleSheet(onStyle.styleSheet());
            } else {
                setStyleSheet(offStyle.styleSheet());
            }
        } else {
            setStyleSheet("");
        }
    } else {
        setStyleSheet("");
    }
}
