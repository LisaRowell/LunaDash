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

#ifndef WIDGET_STYLE_H
#define WIDGET_STYLE_H

#include "XMLSourcedEntity.h"
#include "XMLFileReader.h"


#include <QVector>
#include <QString>

class WidgetStyles;

class WidgetStyle : public XMLSourcedEntity {
private:
    QString name_;
    WidgetStyle *base;

    QString backgroundColor_;
    QString color_;
    QString borderWidth_;
    QString borderStyle_;
    QString borderRadius_;
    QString borderColor_;
    QString padding_;
    QString topPadding_;
    QString bottomPadding_;
    QString leftPadding_;
    QString rightPadding_;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

    void setBase(const XMLFileReader &xmlReader,
                 const WidgetStyles &widgetStyles);
    const QString backgroundColor() const;
    const QString color() const;
    const QString borderWidth() const;
    const QString borderStyle() const;
    const QString borderRadius() const;
    const QString borderColor() const;
    const QString padding() const;
    const QString topPadding() const;
    const QString bottomPadding() const;
    const QString leftPadding() const;
    const QString rightPadding() const;
    [[noreturn]] void
    duplicateStyleNamesErrors(const QString &name,
                              const XMLFileReader &xmlReader) const;

public:
    WidgetStyle();
    WidgetStyle(XMLFileReader &xmlReader, WidgetStyles &widgetStyles);
    void set(XMLFileReader &xmlReader, WidgetStyles &widgetStyles);
    const QString &name() const;
    QString styleSheet() const;
};

#endif // WIDGET_STYLE_H
