#ifndef BOOL_VARIABLE_H
#define BOOL_VARIABLE_H

#include "Variable.h"
#include "Variables.h"

#include <QXmlStreamReader>
#include <QString>

class BoolVariable : public Variable {
private:
    bool value_;

public:
    BoolVariable(QXmlStreamReader &xmlReader, const QString &fileName,
                 Variables &variables, bool initialValue = false);
    void set(bool value);
    virtual const QString &string() const override;
};

#endif // BOOL_VARIABLE_H
