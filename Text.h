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

#ifndef TEXT_H
#define TEXT_H

#include "ValuedWidget.h"
#include "Variables.h"
#include "XMLFileReader.h"

#include <QLabel>
#include <QString>

class Text : public QLabel, public ValuedWidget {
private:
    static const QString typeName;

protected:
    virtual void setValue() override;

public:
    Text(XMLFileReader &xmlReader, const Variables &variables);
};

#endif // TEXT_H
