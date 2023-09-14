#ifndef VALUED_WIDGET_H
#define VALUED_WIDGET_H

#include "Widget.h"
#include "Variable.h"
#include "Variables.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include <QObject>

class ValuedWidget : public Widget {
    Q_OBJECT

private:
    void unknownVariableWarning(const QString &widgetType,
                                const QString &variableName,
                                const QXmlStreamReader &xmlReader,
                                const QString &fileName) const;

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

protected:
    Variable *variable;

    virtual void setValue() = 0;

public:
    ValuedWidget(const QString &widgetType, QXmlStreamReader &xmlReader,
                 const QString &fileName, const Variables &variables);

public slots:
    void valueChanged();
};

#endif // VALUED_WIDGET_H
