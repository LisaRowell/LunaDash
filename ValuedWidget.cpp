#include "ValuedWidget.h"

#include "Widget.h"
#include "Variables.h"
#include "Variable.h"

#include <QVector>
#include <QString>
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> ValuedWidget::allowedAttrs = { "variable" };
const QVector<QString> ValuedWidget::requiredAttrs = { };

ValuedWidget::ValuedWidget(const QString &widgetType,
                           QXmlStreamReader &xmlReader,
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
            unknownVariableWarning(widgetType, variableName, xmlReader,
                                   fileName);
        }
    } else {
        variable = NULL;
    }
}

void ValuedWidget::valueChanged() {
    setValue();
}

void ValuedWidget::unknownVariableWarning(const QString &widgetType,
                                          const QString &variableName,
                                          const QXmlStreamReader &xmlReader,
                                          const QString &fileName) const {
    QString errorStr;
    QTextStream errorStream(&errorStr);

    errorStream << widgetType <<" with an unknown variable '" << variableName
                << "' in file '" << fileName << "' ("
                << xmlReader.lineNumber() << ", " << xmlReader.columnNumber()
                << "):" << Qt::endl;
    errorStream << "Ignored.";

    QMessageBox messageBox;
    messageBox.warning(NULL, "Unknown Variable Warning", errorStr);
}
