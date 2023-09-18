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

#include "Widget.h"

#include "XMLSourcedEntity.h"
#include "GridPos.h"

#include <QString>
#include <QVector>
#include <QXmlStreamReader>

Widget::Widget(const QVector<QString> &allowedAttrs,
               const QVector<QString> &requiredAttrs)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
}

void Widget::handleChildElement(QXmlStreamReader &xmlReader,
                                const QString &parentName,
                                const QString &fileName) {
    if (xmlReader.name().compare("GridPos") == 0) {
        gridPos_ = new GridPos(xmlReader, fileName);
    } else {
        unsupportedChildElement(parentName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

const GridPos *Widget::gridPos() const {
    return gridPos_;
}
