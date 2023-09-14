#ifndef VARIABLE_H
#define VARIABLE_H

#include "XMLSourcedEntity.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include <QObject>

class Variables;

class Variable : public XMLSourcedEntity {
    Q_OBJECT;

private:
    QString name_;

    void duplicateVariableNamesErrors(const QString &name,
                                      const QXmlStreamReader &xmlReader,
                                      const QString &fileName);

    static const QVector<QString> allowedAttrs;
    static const QVector<QString> requiredAttrs;

protected:
    void valueChanged();

public:
    Variable(const QString &typeName, QXmlStreamReader &xmlReader,
             const QString &fileName, Variables &variables);
    const QString &name() const;
    virtual const QString &string() const;

signals:
    void valueChangedSignal();
};

#endif // VARIABLE_H
