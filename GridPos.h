#ifndef GRID_POS_H
#define GRID_POS_H

#include "XMLSourcedEntity.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>

class GridPos : public XMLSourcedEntity {
private:
    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;
    bool set;
    unsigned _row;
    unsigned _col;

public:
    GridPos();
    void setFromXML(QXmlStreamReader &xmlReader, const QString &fileName);
    bool isSet();
    unsigned row();
    unsigned col();
};

#endif // GRID_POS_H
