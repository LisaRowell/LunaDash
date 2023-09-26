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

#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include "ValuedWidget.h"
#include "XMLFileReader.h"
#include "Variables.h"
#include "Range.h"

#include <qwt_dial.h>

#include <QString>
#include <QVector>

class GaugeWidget : public QwtDial, public ValuedWidget {
private:
    Range range;
    bool showValueText;
    QString suffix;
    bool precisionSet;
    unsigned precision;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    void initGauge();

protected:
    virtual void setValue() override;
    virtual void drawScaleContents(QPainter* painter, const QPointF& center,
                                   double radius) const override;
    virtual void drawNeedle(QPainter* painter, const QPointF& center,
                            double radius, double direction,
                            QPalette::ColorGroup colorGroup ) const override;
    virtual QSize sizeHint() const override;

public:
    GaugeWidget(XMLFileReader &xmlReader, const Variables &variables);
};

#endif // GAUGE_WIDGET_H
