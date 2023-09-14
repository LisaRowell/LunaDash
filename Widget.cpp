#include "Widget.h"

#include "XMLSourcedEntity.h"

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
        gridPos.setFromXML(xmlReader, fileName);
    } else {
        unsupportedChildElement(parentName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

bool Widget::hasGridPos() const {
    return gridPos.isSet();
}

unsigned Widget::row() const {
    return gridPos.row();
}

unsigned Widget::col() const {
    return gridPos.col();
}
