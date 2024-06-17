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
    shared_ptr<MethodRecord> lookupMethod(const string &name);
    shared_ptr<ClassRecord> lookupClass(const string &name);
    shared_ptr<Record> lookupRecord(const string &name);
    shared_ptr<Record> lookup(const string &name);

    void addImport(shared_ptr<SymbolTable> st);
    void addMainMethod(shared_ptr<MethodRecord> record);

    void resetTable();

    void enterScope(shared_ptr<Record> record);
    void exitScope();
};
