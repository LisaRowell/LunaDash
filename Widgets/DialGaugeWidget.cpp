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

#include "DialGaugeWidget.h"

#include "Range.h"
#include "ScaleSteps.h"
#include "ScaleThreshold.h"
#include "ScaleThresholds.h"
#include "Variable.h"
#include "XMLFileReader.h"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QtMath>
#include <QVector>
#include <QDebug>  // Delete me

const QVector<QString> DialGaugeWidget::additionalAllowedAttrs = {
    "precision", "suffix"
};

DialGaugeWidget::DialGaugeWidget(XMLFileReader &xmlReader,
                                 const Variables &variables)
    : ValuedWidget("DialGauge", xmlReader, variables, additionalAllowedAttrs,
                   emptyAttrsList),
      precisionSet(false) {
    suffix = stringAttribute("suffix", xmlReader);
    precision = unsignedAttribute("precision", xmlReader, &precisionSet);

    while (xmlReader.readNextStartElement()) {
        const QStringView &elementName = xmlReader.name();
        if (elementName.compare("Label") == 0) {
            label = stringElement("text", xmlReader);
        } else if (elementName.compare("Range") == 0) {
            range.set(xmlReader);
        } else if (elementName.compare("Thresholds") == 0) {
            scaleThresholds.set(xmlReader);
        } else if (elementName.compare("ScaleSteps") == 0) {
            scaleSteps.set(xmlReader);
        } else {
            handleChildElement(xmlReader, "DialGaugeWidget");
        }
    }

    setMinimumSize(150, 150);
}

void DialGaugeWidget::setValue() {
    if (variable != NULL && variable->hasValue()) {
        bool valid;
        value = variable->doubleValue(&valid);
        if (!valid) {
            value = 0;
        }
    } else {
        value = 0;
    }

    update();
}

void DialGaugeWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up a drawing area that's 200 on a side, centered at 0,0
    const int side = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    drawOutline(painter);
    drawScale(painter);
    drawNeedle(painter);
    drawNeedleAxle(painter);
    drawValue(painter);
    drawLabel(painter);
    drawThresholds(painter);
}

void DialGaugeWidget::drawOutline(QPainter &painter) {
    static const QRect bounds(-97, -97, 194, 194);

    painter.save();

    QPen pen;
    pen.setStyle(Qt::SolidLine);

    pen.setWidth(6);
    pen.setBrush(Qt::lightGray);
    painter.setPen(pen);
    painter.drawEllipse(bounds);

    pen.setWidth(2);
    pen.setBrush(Qt::gray);
    painter.setPen(pen);
    painter.drawEllipse(bounds);

    painter.restore();
}

void DialGaugeWidget::drawScale(QPainter &painter) {
    drawScaleArc(painter);
    drawScaleMarks(painter);
    drawScaleLabels(painter);
}

void DialGaugeWidget::drawScaleArc(QPainter &painter) {
    static const QRect bounds(-56, -56, 112, 112);

    painter.save();

    QPen pen = QPen(Qt::black);
    pen.setWidth(0);
    painter.setPen(pen);

    painter.drawArc(bounds, startQtAngle, endQtAngle - startQtAngle);

    painter.restore();
}

void DialGaugeWidget::drawScaleMarks(QPainter &painter) {
    painter.save();

    QPen pen = QPen(Qt::black);
    pen.setWidth(0);

    painter.setPen(pen);
    painter.rotate(-startAngle);

    const unsigned majorSteps = scaleSteps.major();
    const unsigned minorSteps = scaleSteps.minor();
    const double majorStepAngle = scaleAngle / majorSteps;
    const double minorStepAngle = majorStepAngle / (minorSteps + 1);
    const unsigned steps = majorSteps * (minorSteps + 1) + 1;

    for (unsigned step = 0; step < steps; step++) {
        if (step % (minorSteps + 1) == 0) {
            painter.drawLine(56, 0, 64, 0);
        } else {
            painter.drawLine(56, 0, 60, 0);
        }
        painter.rotate(minorStepAngle);
    }

    painter.restore();
}

void DialGaugeWidget::drawScaleLabels(QPainter &painter) {
    painter.save();

    QPen pen = QPen(Qt::black);
    painter.setPen(pen);

    unsigned steps = scaleSteps.major();
    const double stepAngle = scaleAngle / steps;
    const double stepAmount = ((double)range.max() - range.min()) / steps;

    double angleInRadians = qDegreesToRadians(startAngle);
    double stepAngleInRadians = qDegreesToRadians(stepAngle);
    double index = range.min();
    for (unsigned labels = steps + 1; labels--;
         angleInRadians += stepAngleInRadians, index += stepAmount) {
        const double centerX = 80 * sin(angleInRadians);
        const double centerY = 80 * -cos(angleInRadians);

        const QString label = QString::number(index);
        QRectF labelRect = painter.fontMetrics().boundingRect(label);
        labelRect.moveTo(centerX - labelRect.width() / 2,
                         centerY - labelRect.height() / 2);

        painter.drawText(labelRect, Qt::AlignCenter, label);
    }

    painter.restore();
}

void DialGaugeWidget::drawNeedle(QPainter &painter) {
    painter.save();

    QPolygon needle;
    needle << QPoint(-4, 0) << QPoint(4, 0) << QPoint(0, -60);
    QPolygon shadow;
    shadow << QPoint(-2, 0) << QPoint(2, 0) << QPoint(0, -58);

    const double valueInRange = qMin(qMax(value, range.min()), range.max());
    const double angularScaleFactor = scaleAngle / (range.max() - range.min());
    const int degreesFromStart = angularScaleFactor *
                                 (valueInRange - range.min());
    painter.rotate(startAngle + degreesFromStart);

    QRadialGradient haloGradient(0, 0, 20, 0, 0);
    haloGradient.setColorAt(0.0, QColor(255, 120, 120));
    haloGradient.setColorAt(1.0, QColor(200, 20, 20));
    QColor color = QColor(Qt::darkRed);
    color.setAlpha(90);
    painter.setPen(color);
    painter.setBrush(haloGradient);
    painter.drawConvexPolygon(needle);

    painter.setBrush(QColor(255, 120, 120));
    painter.drawConvexPolygon(shadow);

    painter.restore();
}

void DialGaugeWidget::drawNeedleAxle(QPainter &painter) {
    painter.save();

    QLinearGradient linearGrad(QPointF(-7, -7), QPointF(14, 14));
    linearGrad.setColorAt(0, Qt::white);
    linearGrad.setColorAt(1, Qt::black);
    painter.setPen(Qt::NoPen);
    painter.setBrush(linearGrad);
    painter.drawEllipse(-7, -7, 14, 14);

    painter.restore();
}


void DialGaugeWidget::drawValue(QPainter &painter) {
    painter.save();

    QString valueString;
    if (precisionSet) {
        valueString = QString::number(value, 'F', precision);
    } else {
        valueString = QString::number(value);
    }
    if (!suffix.isEmpty()) {
        valueString.append(suffix);
    }

    const QColor &displayColor = scaleThresholds.colorForValue(value);
    painter.setPen(displayColor);
    QFont font = painter.font();
    const int pointSize = font.pointSize() + 2;
    font.setPointSize(pointSize);
    font.setBold(true);
    painter.setFont(font);

    QRectF labelRect = painter.fontMetrics().boundingRect(valueString);
    labelRect.moveTo(0 - labelRect.width() / 2, 65 - labelRect.height() / 2);
    painter.drawText(labelRect, Qt::AlignCenter, valueString);

    painter.restore();
}

void DialGaugeWidget::drawLabel(QPainter &painter) {
    painter.save();

    painter.setPen(Qt::black);

    QRectF labelRect = painter.fontMetrics().boundingRect(label);
    labelRect.moveTo(0 - labelRect.width() / 2,
                     42 - labelRect.height() / 2);

    painter.drawText(labelRect, Qt::AlignCenter, label);

    painter.restore();
}

void DialGaugeWidget::drawThresholds(QPainter &painter) {
    static QRect bounds(-50, -50, 100, 100);

    if (scaleThresholds.configured()) {
        painter.save();

        QPen pen;
        pen.setWidth(6);
        pen.setCapStyle(Qt::FlatCap);
        pen.setColor(Qt::red);
        painter.setPen(pen);

        int lastQtAngle = startQtAngle;
        bool atEndOfScale = false;
        for (unsigned thresholdIndex = 0;
             thresholdIndex < scaleThresholds.count() && !atEndOfScale;
             thresholdIndex++) {
            const ScaleThreshold *threshold =
                scaleThresholds.threshold(thresholdIndex);
            const double thresholdValue = threshold->value();
            if (thresholdValue < range.min()) {
                continue;
            } else {
                const QColor &color = threshold->color();
                pen.setColor(color);
                painter.setPen(pen);

                int thresholdQtAngle;
                if (thresholdValue > range.max()) {
                    atEndOfScale = true;
                    thresholdQtAngle = endQtAngle;
                } else {
                    thresholdQtAngle = qtAngleForValue(thresholdValue);
                }

                const int angleDiff = lastQtAngle - thresholdQtAngle;
                painter.drawArc(bounds, lastQtAngle, -angleDiff);
                lastQtAngle = thresholdQtAngle;
            }
        }

        if (!atEndOfScale) {
            pen.setColor(scaleThresholds.overColor());
            painter.setPen(pen);
            const int angleDiff = lastQtAngle - endQtAngle;
            painter.drawArc(bounds, lastQtAngle, -(int)angleDiff);
        }

        painter.restore();
    }
}

int DialGaugeWidget::qtAngleForValue(double value) {
    const int gaugeSpan = startQtAngle - endQtAngle;
    const double amountInRange = value - range.min();
    const double rangeAmount = range.max() - range.min();
    const int qtAngleFromStart = (gaugeSpan * amountInRange) / rangeAmount;
    return startQtAngle - qtAngleFromStart;
}
