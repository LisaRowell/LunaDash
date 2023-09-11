#include "Label.h"

#include "XMLSourcedEntity.h"

#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QTextStream>
#include <QMessageBox>
#include <QVector>
#include <QString>

const QVector<QString> Label::allowedAttrs = { "text" };
const QVector<QString> Label::requiredAttrs = { "text" };

Label::Label(QXmlStreamReader &xmlReader, const QString &fileName)
    : Widget(allowedAttrs, requiredAttrs) {
    // We should have just a single 'text' attribute.
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    setText(attributes.value("text").toString());

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Label", fileName);
    }
}

