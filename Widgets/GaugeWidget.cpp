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

#include "Variable.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

#include <QPainter>
#include <QString>
#include <QVector>

const QVector<QString> GaugeWidget::additionalAllowedAttrs = {
    "precision", "suffix"
};

GaugeWidget::GaugeWidget(XMLFileReader &xmlReader, const Variables &variables)
    : ValuedWidget("Number", xmlReader, variables, additionalAllowedAttrs,
                   emptyAttrsList), showValueText(true) {
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
    setMinimumSize(150, 150);

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

    // Reduce the number of major dividers so that the scale doesn't go to
    // decimal values.
    const unsigned delta = range.max() - range.min();
    const unsigned maxScaleNumbers = delta > 10 ? 10 : delta;
    setScaleMaxMajor(maxScaleNumbers);
    setScaleMaxMinor(2);
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
    (void)center;
    (void)radius;

    if (!showValueText) {
        QRectF rect(boundingRect());
        rect.setTop(scaleInnerRect().bottom());
        rect.setHeight(rect.height() / 2);

        const QColor color = palette().color(QPalette::Text);
        painter->setPen(color);

        const int flags = Qt::AlignVCenter | Qt::AlignHCenter;
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
        QRectF rect(boundingRect());
        rect.setTop(scaleInnerRect().bottom());
        rect.setHeight(rect.height() / 2);

        const QColor color = palette().color(QPalette::Text);
        painter->setPen(color);

        const int flags = Qt::AlignVCenter | Qt::AlignHCenter;
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

QSize GaugeWidget::sizeHint() const {
    return QSize(150, 150);
}
