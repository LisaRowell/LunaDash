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

#include "GaugeWidget.h"

#include "XMLFileReader.h"
#include "Variables.h"

#include <QVector>
#include <QString>
#include <QPainter>

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

const QVector<QString> GaugeWidget::allowedAttrs = {
    "variable", "precision", "suffix"
};
const QVector<QString> GaugeWidget::requiredAttrs = { };

GaugeWidget::GaugeWidget(XMLFileReader &xmlReader, const Variables &variables)
    : ValuedWidget("Number", xmlReader, variables, allowedAttrs,
                   requiredAttrs), showValueText(true) {
    GaugeWidget::setValue();

    suffix = stringAttribute("suffix", xmlReader);
    precision = unsignedAttribute("precision", xmlReader, &precisionSet);

    while (xmlReader.readNextStartElement()) {
        QStringView elementName = xmlReader.name();
        if (elementName.compare("Range") == 0) {
            range.set(xmlReader);
        } else {
            handleChildElement(xmlReader, "Label");
        }
    }

    initGauge();
}

void GaugeWidget::initGauge() {
    setOrigin(135.0);
    setScaleArc(0.0, 270.0);

    QwtDialSimpleNeedle *needle;
    needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, false,
                                     Qt::red, QColor(Qt::gray).lighter(130));
    setNeedle(needle);

    setReadOnly(true);

    if (range.isSet()) {
        setScale(range.min(), range.max());
    }
}

void GaugeWidget::setValue() {
    if (variable != NULL && variable->hasValue()) {
        bool valid;
        double value = variable->doubleValue(&valid);
        if (valid) {
            QwtDial::setValue(value);
        } else {
            QwtDial::setValue(0);
        }
    } else {
        QwtDial::setValue(0);
    }
}

// If the widget has a label inside the dial, but not a text version of the
// value, we display the text here as it is call less frequently than the
// needle update and it doesn't make sense to keep redrawing it.
void GaugeWidget::drawScaleContents(QPainter* painter, const QPointF& center,
                                    double radius) const {
    if (!showValueText) {
        // Lifted from the Qwt Dial example code
        QRectF rect(0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0);
        rect.moveCenter(center);

        const QColor color = palette().color(QPalette::Text);
        painter->setPen(color);

        const int flags = Qt::AlignBottom | Qt::AlignHCenter;
        painter->drawText(rect, flags, suffix);
    }
}

// We override the dial's needle drawing call as this is invoked anytime the
// value is changed, giving us the opportunity to draw a text value (if
// configured).
void GaugeWidget::drawNeedle(QPainter* painter, const QPointF& center,
                             double radius, double direction,
                             QPalette::ColorGroup colorGroup ) const {
    QwtDial::drawNeedle(painter, center, radius, direction, colorGroup);

    if (showValueText) {
        // Taken from Qwt example code's drawScaleContents and moved to this
        // hook.
        QRectF rect(0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0);
        rect.moveCenter(center);

        const QColor color = palette().color(QPalette::Text);
        painter->setPen(color);

        const int flags = Qt::AlignBottom | Qt::AlignHCenter;
        QString stringValue;
        if (precisionSet) {
            stringValue = QString::number(value(), 'F', precision);
        } else {
            stringValue = QString::number(value());
        }
        if (!suffix.isEmpty()) {
            stringValue.append(suffix);
        }
        painter->drawText(rect, flags, stringValue);
    }
}