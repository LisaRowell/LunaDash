#ifndef TEXT_H
#define TEXT_H

#include "ValuedWidget.h"
#include "Variables.h"

#include <QLabel>
#include <QXmlStreamReader>
#include <QString>

class Text : public QLabel, public ValuedWidget {
protected:
    virtual void setValue() override;

public:
    Text(QXmlStreamReader &xmlReader, const QString &fileName,
         const Variables &variables);
};

#endif // TEXT_H
