#include "GridPos.h"

#include <QVector>
#include <QString>

const QVector<QString> GridPos::allowedAttrs = { "row", "col" };
const QVector<QString> GridPos::requiredAttrs = { "row", "col" };

GridPos::GridPos()
    : XMLSourcedEntity(allowedAttrs, requiredAttrs), set(false), _row(0),
    _col(0) {
}

void GridPos::setFromXML(QXmlStreamReader &xmlReader,
                         const QString &fileName) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    _row = attributes.value("row").toUInt();
    _col = attributes.value("col").toUInt();
    set = true;

    ignoreChildElements(xmlReader, "GridPos", fileName);
}

bool GridPos::isSet() const {
    return set;
}

unsigned GridPos::row() const {
    return _row;
}

unsigned GridPos::col() const {
    return _col;
}
