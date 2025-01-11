/*  WAND - Wand Programming Language compiler
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  Part of structure where all symbols (variables, methods, classes) in code are remembered. It represents function, class, or block of code in {}
 */

#include "Scope.hpp"

Scope::Scope(shared_ptr<Scope> _parent) : parent(_parent) {}

Scope::Scope() {}

void Scope::setScopeNameAndType(string name, string type) {
    scopeName = name;
    scopeType = type;
}

shared_ptr<VarRecord> Scope::lookupVar(const string &name) {
    for (shared_ptr<VarRecord> r : varRecords) {
        if (r->id == name) {
            return r;
        }
    }

    if (parent != nullptr) {
        return parent->lookupVar(name);
    } else {
        return nullptr;
    }
}

shared_ptr<MethodRecord> Scope::lookupMethod(const string &name) {
    for (shared_ptr<MethodRecord> r : methodRecords) {
        if (r->id == name) {
            return r;
        }
    }

    if (parent != nullptr) {
        return parent->lookupMethod(name);
    } else {
        return nullptr;
    }
}

shared_ptr<ClassRecord> Scope::lookupClass(const string &name) {
    for (shared_ptr<ClassRecord> r : classRecords) {
        if (r->id == name) {
            return r;
        }
    }

    if (parent != nullptr) {
        return parent->lookupClass(name);
    } else {
        return nullptr;
    }
}

shared_ptr<Record> Scope::lookupRecord(const string &name) {
    for (shared_ptr<Record> r : records) {
        if (r->id == name) {
            return r;
        }
    }

    if (parent != nullptr) {
        return parent->lookupRecord(name);
    } else {
        return nullptr;
    }
}

shared_ptr<Record> Scope::lookup(const string &name) {
    shared_ptr<VarRecord> varRecord = lookupVar(name);
    shared_ptr<MethodRecord> methodRecord = lookupMethod(name);
    shared_ptr<ClassRecord> classRecord = lookupClass(name);
    shared_ptr<Record> record = lookupRecord(name);
    if (varRecord != nullptr) {
        return varRecord;
    } else if (methodRecord != nullptr) {
        return methodRecord;
    } else if (classRecord != nullptr) {
        return classRecord;
    } else {
        return record;
    }
}

void Scope::put(shared_ptr<VarRecord> varRecord) {
    varRecords.push_back(varRecord);
}

void Scope::put(shared_ptr<MethodRecord> methodRecord) {
    for (auto existing : methodRecords) {
        if (existing->id == methodRecord->id) {
            methodRecord->similar.push_back(existing);
            existing->similar.push_back(methodRecord);
        }
    }

    methodRecords.push_back(methodRecord);
}

void Scope::put(shared_ptr<ClassRecord> classRecord) {
    classRecords.push_back(classRecord);
}

void Scope::put(shared_ptr<Record> record) { records.push_back(record); }

shared_ptr<Scope> Scope::nextChild(shared_ptr<Record> record) {
    shared_ptr<Scope> nextChild;
    if (next >= children.size()) {
        record->next = this->record;
        nextChild = make_shared<Scope>(shared_from_this());
        nextChild->record = record;
        children.push_back(nextChild);
    } else {
        nextChild = children[next];
    }
    next++;
    return nextChild;
}

void Scope::resetScope() {
    next = 0;
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->resetScope();
    }
}
