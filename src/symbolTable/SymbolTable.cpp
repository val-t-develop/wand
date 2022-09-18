#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {
  root = make_shared<Scope>(nullptr);
  this->currentScope = root;

  put(make_shared<ClassRecord>("boolean", "primitive"));
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

shared_ptr<VarRecord> SymbolTable::lookupVar(string name) {
  shared_ptr<VarRecord> curr = currentScope->lookupVar(name);
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

shared_ptr<MethodRecord> SymbolTable::lookupMethod(string name) {
  shared_ptr<MethodRecord> curr = currentScope->lookupMethod(name);
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

shared_ptr<ClassRecord> SymbolTable::lookupClass(string name) {
  shared_ptr<ClassRecord> curr = currentScope->lookupClass(name);
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

shared_ptr<Record> SymbolTable::lookup(string name) {
  shared_ptr<VarRecord> varRecord = lookupVar(name);
  shared_ptr<MethodRecord> methodRecord = lookupMethod(name);
  shared_ptr<ClassRecord> classRecord = lookupClass(name);
  if (varRecord != nullptr) {
    return varRecord;
  } else if (methodRecord != nullptr) {
    return methodRecord;
  } else {
    return classRecord;
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