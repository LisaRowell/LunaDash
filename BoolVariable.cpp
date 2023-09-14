#include "BoolVariable.h"

#include "Variables.h"

#include <QXmlStreamReader>
#include <QString>

BoolVariable::BoolVariable(QXmlStreamReader &xmlReader, const QString &fileName,
                           Variables &variables, bool initialValue)
    : Variable("BoolVariable", xmlReader, fileName, variables),
      value_(initialValue) {
}

void BoolVariable::set(bool value) {
    value_ = value;
    valueChanged();
}

const QString &BoolVariable::string() const {
    if (value_) {
        static QString trueStr("true");
        return trueStr;
    } else {
        static QString falseStr("false");
        return falseStr;
    }
}
