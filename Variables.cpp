#include "Variables.h"

#include <QString>

Variables::Variables() {
}

bool Variables::variableExists(const QString &name) const {
    return variables.contains(name);
}

void Variables::addVariable(Variable *variable) {
    variables.insert(variable->name(), variable);
}

Variable *Variables::find(const QString &name) const {
    auto variableIter = variables.find(name);
    if (variableIter != variables.end()) {
        return variableIter.value();
    } else {
        return NULL;
    }
}
