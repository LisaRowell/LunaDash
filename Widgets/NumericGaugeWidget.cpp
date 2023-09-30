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

#include "NumericGaugeWidget.h"

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QVector>

const QVector<QString> NumericGaugeWidget::additionalAllowedAttrs = {
    "precision", "suffix"
};

NumericGaugeWidget::NumericGaugeWidget(XMLFileReader &xmlReader,
                                       const Variables &variables)
    : ValuedWidget("Number", xmlReader, variables, additionalAllowedAttrs,
                   emptyAttrsList),
    precisionSet(false) {
    layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    valueWidget = new QLabel();
    valueWidget->setStyleSheet("font-size: 28px;border: 4px ridge grey;border-radius: 8");
    valueWidget->setMinimumSize(QSize(160, 45));
    valueWidget->setMaximumSize(QSize(160, 45));
    valueWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(valueWidget, Qt::AlignTop);

    NumericGaugeWidget::setValue();

    suffix = stringAttribute("suffix", xmlReader);
    precision = unsignedAttribute("precision", xmlReader, &precisionSet);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Label") == 0) {
            addLabel(xmlReader);
        } else {
            handleChildElement(xmlReader, "NumericGauge");
        }
    }

    setLayout(layout);
}

void NumericGaugeWidget::addLabel(XMLFileReader &xmlReader) {
    const QString labelText = stringElement("text", xmlReader);
    QLabel *label = new QLabel();
    label->setText(labelText);
    label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(label, Qt::AlignTop);
}

void NumericGaugeWidget::setValue() {
    if (variable != NULL && variable->hasValue()) {
        bool valid;
        double value = variable->doubleValue(&valid);
        if (valid) {
            QString stringValue;
            if (precisionSet) {
                stringValue = QString::number(value, 'F', precision);
            } else {
                stringValue = QString::number(value);
            }
            if (!suffix.isEmpty()) {
                stringValue.append(suffix);
            }
            valueWidget->setText(stringValue);
        } else {
            valueWidget->setText("");
        }
    } else {
        valueWidget->setText("");
    }
}
