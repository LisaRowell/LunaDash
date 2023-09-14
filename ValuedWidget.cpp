#include "ValuedWidget.h"

#include "Widget.h"
#include "Variables.h"
#include "Variable.h"

#include <QVector>
#include <QString>
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

const QVector<QString> ValuedWidget::allowedAttrs = { "variable" };
const QVector<QString> ValuedWidget::requiredAttrs = { };

ValuedWidget::ValuedWidget(QXmlStreamReader &xmlReader,
                           const QString &fileName,
                           const Variables &variables)
    : Widget(allowedAttrs, requiredAttrs) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);

    if (attributes.hasAttribute("variable")) {
        const QString &variableName = attributes.value("variable").toString();
        variable = variables.find(variableName);
        if (variable != NULL) {
            connect(variable, SIGNAL(valueChangedSignal()),
                    this, SLOT(valueChanged()));
        } else {
            unknownVariableWarning();
        }
    } else {
        variable = NULL;
    }
}

void ValuedWidget::valueChanged() {
    setValue();
}

void ValuedWidget::unknownVariableWarning() {
}
