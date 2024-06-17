#include "MethodRecord.hpp"
#include <utils/Out.hpp>

MethodRecord::MethodRecord(string _id, string _type)
    : Record(_id, _type, RecordKind::METHOD_RECORD) {}

bool MethodRecord::equals(shared_ptr<MethodRecord> r) {
    bool isVarsEqual = true;
    if (vars.size() == r->vars.size()) {
        for (size_t i = 0; i < vars.size(); ++i) {
            if (vars[i] != nullptr && r->vars[i] != nullptr)
                if (!vars[i]->equals(r->vars[i])) {
                    isVarsEqual = false;
                }
        }
    }
    return id == r->id && type == r->type &&
           (next == nullptr ? r->next == nullptr : next->equals(r->next)) &&
           argsCount == r->argsCount && isConstructor == r->isConstructor &&
           isVarsEqual;
}

void MethodRecord::addVar(shared_ptr<VarRecord> var) { vars.push_back(var); }

void MethodRecord::addArg(shared_ptr<VarRecord> var) {
    argsCount++;
    vars.push_back(var);
}

bool MethodRecord::containArg(shared_ptr<VarRecord> var) {
    for (int i = 0; i < argsCount; ++i) {
        if (vars[i]->equals(var)) {
            return true;
        }
    }
    return false;
}

bool MethodRecord::containArg(shared_ptr<VarRecord> var, int n) {
    if (n <= argsCount) {
        if (vars[n]->equals(var)) {
            return true;
        }
    }
    return false;
}

string MethodRecord::getFullName() {
    if (ir_name == "") {
        Out::errorMessage(
            "Internal error detected! Can not get ir_name of method " + id);
    }
    return ir_name;
}
