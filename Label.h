#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include <QLabel>
#include <QXmlStreamReader>
#include <QString>
#include <QXmlStreamAttribute>
#include <QVector>

class Label : public QLabel, public Widget {
private:
    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

public:
    Label(QXmlStreamReader &xmlReader, const QString &fileName);
};

#endif // LABEL_H
