#include "XMLSourcedEntity.h"

#include <QVector>
#include <QString>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QMessageBox>

XMLSourcedEntity::XMLSourcedEntity(const QVector<QString> &allowedAttrs,
                                   const QVector<QString> &requiredAttrs)
    : allowedAttrs(allowedAttrs), requiredAttrs(requiredAttrs){
}

void XMLSourcedEntity::checkAttrs(const QXmlStreamAttributes &attributes,
                                  const QString &fileName,
                                  const QXmlStreamReader &xmlReader) {
    QVector<QString> requiredNotFound(requiredAttrs);

    for (auto iterator = attributes.begin(); iterator != attributes.end();
         ++iterator) {
        const QXmlStreamAttribute &attribute = *iterator;

        if (!allowedAttrs.contains(attribute.name())) {
            unsupportedAttrError(attribute, fileName, xmlReader);
        }

        requiredNotFound.removeOne(attribute.name());
    }

    if (requiredNotFound.count()) {
        missingRequiredAttrError(requiredNotFound.first(), fileName, xmlReader);
    }
}

void XMLSourcedEntity::ignoreChildElements(QXmlStreamReader &xmlReader,
                                           const QString &parentName,
                                           const QString &fileName) {
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(parentName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

void XMLSourcedEntity::unsupportedChildElement(const QString &parentName,
                                               const QString &fileName,
                                               const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Unsupported " << parentName << " child element '"
                << xmlReader.name() << "' found in file '" << fileName
                << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Child Element", errorStr);
}

void XMLSourcedEntity::unsupportedAttrError(const QXmlStreamAttribute &attribute,
                                            const QString &fileName,
                                            const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Unsupported " << xmlReader.name() << " attribute '"
                << attribute.name() << "' found in file '" << fileName
                << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ")." << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unsupported Attribute", errorStr);
}

void XMLSourcedEntity::missingRequiredAttrError(const QString &attributeName,
                                                const QString &fileName,
                                                const QXmlStreamReader &xmlReader) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Missing " << xmlReader.name() << " required attribute "
                << attributeName << " in file '" << fileName << "' ("
                << xmlReader.lineNumber() << "," << xmlReader.columnNumber()
                << ").";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Missing Attribute", errorStr);
    exit(EXIT_FAILURE);
}
