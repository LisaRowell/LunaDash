#include "Text.h"

#include "ValuedWidget.h"
#include "Variable.h"
#include "Variables.h"

#include <QVector>
#include <QString>

Text::Text(QXmlStreamReader &xmlReader, const QString &fileName,
           const Variables &variables)
    : ValuedWidget(xmlReader, fileName, variables) {

    if (variable != NULL) {
        setText(variable->string());
    } else {
        setText("");
    }

    // Loop through the child elements, any that are there are for Widget
    while (xmlReader.readNextStartElement()) {
        handleChildElement(xmlReader, "Text", fileName);
    }
}

void Text::setValue() {
    if (variable != NULL) {
        setText(variable->string());
    }
}
