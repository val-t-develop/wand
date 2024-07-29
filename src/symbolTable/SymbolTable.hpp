/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022-2024  Valentyn Tymchyshyn
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
 *  Structure where all symbols (variables, methods, classes) in code are remembered.
 */

#pragma once
#include "Scope.hpp"
#include <Defs.hpp>

class SymbolTable {
  public:
    shared_ptr<Scope> root;
    vector<shared_ptr<SymbolTable>> imports = vector<shared_ptr<SymbolTable>>();
    shared_ptr<Scope> currentScope;
    vector<shared_ptr<MethodRecord>> mainMethods =
        vector<shared_ptr<MethodRecord>>();

    SymbolTable();

    string getCurrentClassName();
    void setCurrentScopeNameAndType(string name, string type);
    void setCurrentScopeClass(shared_ptr<ClassRecord> containingClass);

    void put(shared_ptr<VarRecord> varRecord);
    void put(shared_ptr<MethodRecord> methodRecord);
    void put(shared_ptr<ClassRecord> classRecord);
    void put(shared_ptr<Record> record);

    shared_ptr<VarRecord> lookupVar(const string &name);
    shared_ptr<VarRecord> lookupVarNonRecursive(const string &name);
    shared_ptr<MethodRecord> lookupMethod(const string &name);
    shared_ptr<MethodRecord> lookupMethodNonRecursive(const string &name);
    shared_ptr<ClassRecord> lookupClass(const string &name);
    shared_ptr<ClassRecord> lookupClassNonRecursive(const string &name);
    shared_ptr<Record> lookupRecord(const string &name);
    shared_ptr<Record> lookupRecordNonRecursive(const string &name);
    shared_ptr<Record> lookup(const string &name);

    void addImport(shared_ptr<SymbolTable> st);
    void addMainMethod(shared_ptr<MethodRecord> record);

    void resetTable();

    void enterScope(shared_ptr<Record> record);
    void exitScope();
};
