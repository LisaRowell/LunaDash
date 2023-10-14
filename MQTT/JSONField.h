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

#ifndef JSON_FIELD_H
#define JSON_FIELD_H

#include "XMLSourcedEntity.h"

#include <QObject>
#include <QJsonValue>
#include <QString>

class Variables;
class XMLFileReader;

class QJsonObject;

class JSONField : public QObject, public XMLSourcedEntity {
    Q_OBJECT

private:
    QString name;
    QJsonValue::Type type;

    void addStringVariable(XMLFileReader &xmlReader, Variables &variables);
    void addDoubleVariable(XMLFileReader &xmlReader, Variables &variables);
    void addBoolVariable(XMLFileReader &xmlReader, Variables &variables);
    void receivedValueForString(QJsonValue &value);
    void receivedValueForDouble(QJsonValue &value);
    void receivedValueForBool(QJsonValue &value);
    void multipleVariableWarning(XMLFileReader &xmlReader) const;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

public:
    JSONField(XMLFileReader &xmlReader, Variables &variables);
    void objectReceived(QJsonObject &object);

signals:
    void stringFieldChangedSignal(QString value);
    void doubleFieldChangedSignal(double value);
    void boolFieldChangedSignal(bool value);
    void resetFieldSignal();
};

#endif // JSON_FIELD_H
