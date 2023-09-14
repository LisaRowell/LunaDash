#include "ClientStatusVariable.h"

#include "Variables.h"

#include <QXmlStreamReader>
#include <QString>

ClientStatusVariable::ClientStatusVariable(QXmlStreamReader &xmlReader,
                                           const QString &fileName,
                                           Variables &variables)
    : BoolVariable(xmlReader, fileName, variables) {
}
