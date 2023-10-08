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

#ifndef DIAL_GAUGE_WIDGET_H
#define DIAL_GAUGE_WIDGET_H

#include "ValuedWidget.h"

#include "Range.h"
#include "ScaleSteps.h"
#include "ScaleThresholds.h"

#include <QString>
#include <QVector>
#include <QWidget>

class DialGaugeWidget : public QWidget, public ValuedWidget  {
private:
    Range range;
    ScaleSteps scaleSteps;
    QString suffix;
    bool precisionSet;
    unsigned precision;
    QString label;
    ScaleThresholds scaleThresholds;
    double value;

    void drawOutline(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawScaleArc(QPainter &painter);
    void drawScaleMarks(QPainter &painter);
    void drawScaleLabels(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawNeedleAxle(QPainter &painter);
    void drawValue(QPainter &painter);
    void drawLabel(QPainter &painter);
    void drawThresholds(QPainter &painter);
    int qtAngleForValue(double value);

    static const int startAngle = 225;
    // Qt angles increase counter clockwise with 0 at 3:00
    static const int startQtAngle = ((360 - 225) + 90) * 16;
    static const int endAngle = 135;
    // We use a negative end angle to make math easier
    static const int endQtAngle = (90 - 135) * 16;
    static const int oldEndAngle = -45;
    static const int scaleAngle = endAngle - startAngle + 360;
    static const QVector<QString> additionalAllowedAttrs;

protected:
    virtual void setValue() override;
    virtual void paintEvent(QPaintEvent *event) override;

public:
    DialGaugeWidget(XMLFileReader &xmlReader, const Variables &variables);
};

#endif // DIAL_GAUGE_WIDGET_H
