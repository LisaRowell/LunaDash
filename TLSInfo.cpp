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

#include "TLSInfo.h"

#include "XMLFileReader.h"
#include "XMLSourcedEntity.h"

#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

const QVector<QString> TLSInfo::allowedAttrs = { };
const QVector<QString> TLSInfo::requiredAttrs = { };

TLSInfo::TLSInfo()
    : XMLSourcedEntity(allowedAttrs, requiredAttrs),  set_(false) {
    makeCStrings();
}

void TLSInfo::set(XMLFileReader &xmlReader, QString &serverName) {
    if (set_) {
        multipleTLSWarning(xmlReader, serverName);
        xmlReader.skipCurrentElement();
    } else {
        checkAttrs(xmlReader);

        set_ = true;

        while (xmlReader.readNextStartElement()) {
            QStringView elementName = xmlReader.name();
            if (elementName.compare("serverCertificate") == 0) {
                serverCertificateFileName = stringElement("file", xmlReader);
            } else {
                unsupportedChildElement("TLS", xmlReader);
                xmlReader.skipCurrentElement();
            }
        }

        makeCStrings();
    }
}

bool TLSInfo::tlsConfigured() const {
    return set_;
}

const char *TLSInfo::serverCertificateFileNameCStr() const {
    if (serverCertificateFileName.isEmpty()) {
        return NULL;
    } else {
        return serverCertificateFileNameBA.data();
    }
}

void TLSInfo::makeCStrings() {
    serverCertificateFileNameBA = serverCertificateFileName.toLocal8Bit();
}

void TLSInfo::multipleTLSWarning(XMLFileReader &xmlReader,
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
