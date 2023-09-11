#ifndef WIDGET_H
#define WIDGET_H

#include "XMLSourcedEntity.h"
#include "GridPos.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>

class Widget : public XMLSourcedEntity {
private:
    GridPos gridPos;

public:
    Widget(const QVector<QString> &allowedAttrs,
           const QVector<QString> &requiredAttrs);
    void handleChildElement(QXmlStreamReader &xmlReader,
                            const QString &parentName,
                            const QString &fileName);
    bool hasGridPos();
    unsigned row();
    unsigned col();
};

#endif // WIDGET_H
