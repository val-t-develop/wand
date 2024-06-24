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
 *  Structure where all symbols (variables, methods, classes) in code are remembered.
 */

#include "SymbolTable.hpp"

SymbolTable::SymbolTable() : root(make_shared<Scope>()), currentScope(root) {
    put(make_shared<ClassRecord>("bool", "primitive"));
    put(make_shared<ClassRecord>("int", "primitive"));
    put(make_shared<ClassRecord>("byte", "primitive"));
    put(make_shared<ClassRecord>("short", "primitive"));
    put(make_shared<ClassRecord>("long", "primitive"));
    put(make_shared<ClassRecord>("float", "primitive"));
    put(make_shared<ClassRecord>("double", "primitive"));
    put(make_shared<ClassRecord>("char", "primitive"));
    put(make_shared<ClassRecord>("void", "primitive"));
}

string SymbolTable::getCurrentClassName() {
    return currentScope->containingClass->id;
}

void SymbolTable::setCurrentScopeNameAndType(string name, string type) {
    currentScope->setScopeNameAndType(name, type);
}

void SymbolTable::setCurrentScopeClass(
    shared_ptr<ClassRecord> containingClass) {
    currentScope->containingClass = containingClass;
}

void SymbolTable::put(shared_ptr<VarRecord> varRecord) {
    currentScope->put(varRecord);
}

void SymbolTable::put(shared_ptr<MethodRecord> methodRecord) {
    currentScope->put(methodRecord);
}

void SymbolTable::put(shared_ptr<ClassRecord> classRecord) {
    currentScope->put(classRecord);
}

void SymbolTable::put(shared_ptr<Record> record) { currentScope->put(record); }

shared_ptr<VarRecord> SymbolTable::lookupVar(const string &name) {
    shared_ptr<VarRecord> curr = nullptr;
    if (currentScope != nullptr) {
        curr = currentScope->lookupVar(name);
    }
    if (curr != nullptr) {
        return curr;
    }
    for (shared_ptr<SymbolTable> st : imports) {
        shared_ptr<VarRecord> inImport = st->lookupVar(name);
        if (inImport != nullptr) {
            return inImport;
        }
    }
    return nullptr;
}

shared_ptr<MethodRecord> SymbolTable::lookupMethod(const string &name) {
    shared_ptr<MethodRecord> curr = nullptr;
    if (currentScope != nullptr) {
        curr = currentScope->lookupMethod(name);
    }
    if (curr != nullptr) {
        return curr;
    }
    for (shared_ptr<SymbolTable> st : imports) {
        shared_ptr<MethodRecord> inImport = st->lookupMethod(name);
        if (inImport != nullptr) {
            return inImport;
        }
    }
    return nullptr;
}

shared_ptr<ClassRecord> SymbolTable::lookupClass(const string &name) {
    shared_ptr<ClassRecord> curr = nullptr;
    if (currentScope != nullptr) {
        curr = currentScope->lookupClass(name);
    }
    if (curr != nullptr) {
        return curr;
    }
    for (shared_ptr<SymbolTable> st : imports) {

        shared_ptr<ClassRecord> inImport = st->lookupClass(name);
        if (inImport != nullptr) {
            return inImport;
        }
    }
    return nullptr;
}

shared_ptr<Record> SymbolTable::lookupRecord(const string &name) {
    shared_ptr<Record> curr = nullptr;
    if (currentScope != nullptr) {
        curr = currentScope->lookupRecord(name);
    }
    if (curr != nullptr) {
        return curr;
    }
    for (shared_ptr<SymbolTable> st : imports) {
        shared_ptr<Record> inImport = st->lookupRecord(name);
        if (inImport != nullptr) {
            return inImport;
        }
    }
    return nullptr;
}

shared_ptr<Record> SymbolTable::lookup(const string &name) {
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

void SymbolTable::addImport(shared_ptr<SymbolTable> st) {
    imports.push_back(st);
}

void SymbolTable::addMainMethod(shared_ptr<MethodRecord> record) {
    mainMethods.push_back(record);
}

void SymbolTable::resetTable() { root->resetScope(); }

void SymbolTable::enterScope(shared_ptr<Record> record) {
    currentScope = currentScope->nextChild(record);
}

void SymbolTable::exitScope() { currentScope = currentScope->parent; }
