#ifndef VARIABLES_H
#define VARIABLES_H

#include "Variable.h"

#include <QMap>
#include <QString>

class Variables {
private:
    QMap<QString, Variable *> variables;

public:
    Variables();
    bool variableExists(const QString &name) const;
    void addVariable(Variable *variable);
    Variable *find(const QString &name) const;
};

#endif // VARIABLES_H
