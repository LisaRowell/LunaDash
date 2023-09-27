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

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "XMLSourcedEntity.h"
#include "Variable.h"
#include "Variables.h"

#include <QVector>
#include <QString>

class Transformer : public QObject, public XMLSourcedEntity {
    Q_OBJECT

private:
    void addStringVariable(XMLFileReader &xmlReader, Variables &variables);
    void unknownVariableWarning(const QString &transformerName,
                                const QString &variableName,
                                const XMLFileReader &xmlReader) const;

protected:
    Variable *findSourceVariable(const QString &attributeName,
                                 const QString transformerName,
                                 const XMLFileReader &xmlReader,
                                 const Variables &variables);
    void parseChildElements(XMLFileReader &xmlReader, Variables &variables);
    void publishResult(const QString &result);
    virtual void recalculate() = 0;

public:
    Transformer(const QVector<QString> &allowedAttrs,
                const QVector<QString> &requiredAttrs);

public slots:
    void inputChanged();

signals:
    void newStringValueSignal(const QString &description);
};

#endif // TRANSFORMER_H
