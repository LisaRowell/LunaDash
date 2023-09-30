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

#include "UserId.h"

#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

const QVector<QString> UserId::allowedAttrs = { "name", "password" };
const QVector<QString> UserId::requiredAttrs = { "name" };

UserId::UserId() : set_(false) {
    makeCStrings();
}

void UserId::set(XMLFileReader &xmlReader, QString &serverName) {
    if (set_) {
        multipleUserWarning(xmlReader, serverName);
        xmlReader.skipCurrentElement();
    } else {
        checkAttrs(xmlReader, allowedAttrs, requiredAttrs);

        set_ = true;
        name = stringAttribute("name", xmlReader);
        password = stringAttribute("password", xmlReader);
        makeCStrings();

        ignoreChildElements(xmlReader, "UserId");
    }
}

void UserId::makeCStrings() {
    nameByteArray = name.toLocal8Bit();
    passwordByteArray = password.toLocal8Bit();
}

const char *UserId::nameCString() const {
    if (name.isEmpty()) {
        return NULL;
    } else {
        return nameByteArray.data();
    }
}

const char *UserId::passwordCString() const {
    if (password.isEmpty()) {
        return NULL;
    } else {
        return passwordByteArray.data();
    }
}

void UserId::multipleUserWarning(XMLFileReader &xmlReader,
                                 QString &serverName) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Multiple Users specified for server '" << serverName
                  << "' in file " << xmlReader.fileReference() << "."
                  << Qt::endl;
    warningStream << "Ignoring all but the first.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Multiple Users Warning", warningStr);
}
