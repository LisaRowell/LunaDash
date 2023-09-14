#ifndef CLIENT_STATUS_VARIABLE_H
#define CLIENT_STATUS_VARIABLE_H

#include "BoolVariable.h"

#include "Variables.h"

#include <QXmlStreamReader>
#include <QString>

class ClientStatusVariable : public BoolVariable {
public:
    ClientStatusVariable(QXmlStreamReader &xmlReader, const QString &fileName,
                         Variables &variables);
};

#endif // CLIENT_STATUS_VARIABLE_H
