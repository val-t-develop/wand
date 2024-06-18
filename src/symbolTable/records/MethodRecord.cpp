/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  Symbol of method.
 */

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
