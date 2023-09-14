#include "Variable.h"

#include "Variables.h"

#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QTextStream>
#include <QMessageBox>

const QVector<QString> Variable::allowedAttrs = { "name" };
const QVector<QString> Variable::requiredAttrs = { "name" };

Variable::Variable(const QString &typeName, QXmlStreamReader &xmlReader,
                   const QString &fileName, Variables &variables)
    : XMLSourcedEntity(allowedAttrs, requiredAttrs) {
    const QXmlStreamAttributes &attributes = xmlReader.attributes();
    checkAttrs(attributes, fileName, xmlReader);
    name_ = attributes.value("name").toString();

    // Make sure that the user hasn't defined multiple variables with the same
    // name as it's required to be a unique
    if (variables.variableExists(name_)) {
        duplicateVariableNamesErrors(name_, xmlReader, fileName);
    }

    // Loop through the child elements
    while (xmlReader.readNextStartElement()) {
        unsupportedChildElement(typeName, fileName, xmlReader);
        xmlReader.skipCurrentElement();
    }
}

const QString &Variable::name() const {
    return name_;
}

void Variable::valueChanged() {
    emit valueChangedSignal();
}

const QString &Variable::string() const {
    static QString emptyString("");
    return emptyString;
}

void Variable::duplicateVariableNamesErrors(const QString &name,
                                            const QXmlStreamReader &xmlReader,
                                            const QString &fileName) {
    QString errorStr;
    QTextStream errorStream(&errorStr);
    errorStream << "Non-unique variable name '" << name << "' found in file '"
                << fileName << "' (" << xmlReader.lineNumber() << ","
                << xmlReader.columnNumber() << ").";

    QMessageBox messageBox;
    messageBox.critical(NULL, "Non-Unique Variable Error", errorStr);
    exit(EXIT_FAILURE);
}
