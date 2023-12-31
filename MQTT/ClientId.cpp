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

#include "ClientId.h"

#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QHostInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QString>
#include <QStringView>
#include <QTextStream>
#include <QVector>

const QVector<QString> ClientId::allowedAttrs = {
    "id", "randomize", "hostPrefix"
};

ClientId::ClientId()
    : set_(false), base("LunaDash"), randomize(false),
      prefixWithHostName(false) {
    buildId();
}

void ClientId::set(XMLFileReader &xmlReader, QString &serverName) {
    if (set_) {
        multipleIdWarning(xmlReader, serverName);
        xmlReader.skipCurrentElement();
    } else {
        checkAttrs(xmlReader, allowedAttrs, emptyAttrsList);

        set_ = true;
        base = stringAttribute("id", xmlReader, "LunaDash");
        randomize = boolAttribute("randomize", xmlReader, false);
        prefixWithHostName = boolAttribute("hostPrefix", xmlReader, false);
        buildId();

        ignoreChildElements(xmlReader, "ClientId");
    }
}

void ClientId::buildId() {
    idStr.clear();
    QTextStream idStream(&idStr);

    if (prefixWithHostName) {
        idStream << QHostInfo::localHostName() << "-";
    }

    idStream << base;

    if (randomize) {
        quint32 randomNumber = QRandomGenerator::global()->generate();
        randomNumber = randomNumber % 10000;
        QString randomNumberStr = QString::number(randomNumber);
        QString padding = "0000";
        unsigned padCharacters = 4 - randomNumberStr.length();
        idStream << padding.first(padCharacters) << randomNumberStr;
    }

    idByteArray = idStr.toLocal8Bit();
}

const char *ClientId::cString() const {
    return idByteArray.data();
}

void ClientId::multipleIdWarning(XMLFileReader &xmlReader,
                                 QString &serverName) const {
    QString warningStr;
    QTextStream warningStream(&warningStr);

    warningStream << "Multiple ClientIds specified for server '" << serverName
                  << "' in file " << xmlReader.fileReference() << "."
                  << Qt::endl;
    warningStream << "Ignoring all but the first.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Multiple ClientIds Warning", warningStr);
}
