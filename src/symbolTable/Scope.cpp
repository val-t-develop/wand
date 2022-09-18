#include "Scope.hpp"

Scope::Scope(shared_ptr<Scope> parent) : parent(parent) {}

Scope::Scope(Scope* r) {
  Scope(shared_ptr<Scope>(r));
}

void Scope::setScopeNameAndType(string name, string type) {
  scopeName = name;
  scopeType = type;
}

shared_ptr<VarRecord> Scope::lookupVar(string name) {
  if (getIdByKey(varRecords, name) != 0) {
    return varRecords[getIdByKey(varRecords, name) - 1];
  } else {
    if (parent == nullptr) {
      return nullptr;
    } else {
      return parent->lookupVar(name);
    }
  }
}

shared_ptr<MethodRecord> Scope::lookupMethod(string name) {
  if (getIdByKey(methodRecords, name) != 0) {
    return methodRecords[getIdByKey(methodRecords, name) - 1];
  } else {
    if (parent == nullptr) {
      return nullptr;
    } else {
      return parent->lookupMethod(name);
    }
  }
}

shared_ptr<ClassRecord> Scope::lookupClass(string name) {
  if (getIdByKey(classRecords, name) != 0) {
    return classRecords[getIdByKey(classRecords, name) - 1];
  } else {
    if (parent == nullptr) {
      return nullptr;
    } else {
      return parent->lookupClass(name);
    }
  }
}

shared_ptr<Record> Scope::lookup(string name) {
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

void Scope::put(shared_ptr<VarRecord> varRecord) {
  varRecords.push_back(varRecord);
}

void Scope::put(shared_ptr<MethodRecord> methodRecord) {
  methodRecords.push_back(methodRecord);
}

void Scope::put(shared_ptr<ClassRecord> classRecord) {
  classRecords.push_back(classRecord);
}

shared_ptr<Scope> Scope::nextChild(shared_ptr<Record> record) {
  shared_ptr<Scope> nextChild;
  if (next >= children.size()) {
    record->next = this->record;
    nextChild = make_shared<Scope>(this);
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

int Scope::getIdByKey(vector<shared_ptr<VarRecord>> list, string key) {
  int i = 0;
  for (shared_ptr<Record> r : list) {
    i++;
    if (r->id == key) {
      return i;
    }
  }
  return 0;
}

int Scope::getIdByKey(vector<shared_ptr<MethodRecord>> list, string key) {
  int i = 0;
  for (shared_ptr<Record> r : list) {
    i++;
    if (r->id == key) {
      return i;
    }
  }
  return 0;
}

int Scope::getIdByKey(vector<shared_ptr<ClassRecord>> list, string key) {
  int i = 0;
  for (shared_ptr<Record> r : list) {
    i++;
    if (r->id == key) {
      return i;
    }
  }
  return 0;
}