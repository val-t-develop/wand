#include "Scope.hpp"

Scope::Scope(shared_ptr<Scope> _parent) : parent(_parent) {}

Scope::Scope() {}

void Scope::setScopeNameAndType(string name, string type) {
    scopeName = name;
    scopeType = type;
}

shared_ptr<VarRecord> Scope::lookupVar(const string &name) {
    for(shared_ptr<VarRecord> r : varRecords) {
        if(r->id == name) {
            return r;
        }
    }

    if(parent != nullptr) {
        return parent->lookupVar(name);
    } else {
        return nullptr;
    }
}

shared_ptr<MethodRecord> Scope::lookupMethod(const string &name) {
    for(shared_ptr<MethodRecord> r : methodRecords) {
        if(r->id == name) {
            return r;
        }
    }

    if(parent != nullptr) {
        return parent->lookupMethod(name);
    } else {
        return nullptr;
    }
}

shared_ptr<ClassRecord> Scope::lookupClass(const string &name) {
    for(shared_ptr<ClassRecord> r : classRecords) {
        if(r->id == name) {
            return r;
        }
    }

    if(parent != nullptr) {
        return parent->lookupClass(name);
    } else {
        return nullptr;
    }
}

shared_ptr<Record> Scope::lookupRecord(const string &name) {
    for(shared_ptr<Record> r : records) {
        if(r->id == name) {
            return r;
        }
    }

    if(parent != nullptr) {
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
    if(varRecord != nullptr) {
        return varRecord;
    } else if(methodRecord != nullptr) {
        return methodRecord;
    } else if(classRecord != nullptr) {
        return classRecord;
    } else {
        return record;
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

void Scope::put(shared_ptr<Record> record) {
    records.push_back(record);
}

shared_ptr<Scope> Scope::nextChild(shared_ptr<Record> record) {
    shared_ptr<Scope> nextChild;
    if(next >= children.size()) {
        record->next = this->record;
        nextChild = make_shared<Scope> (shared_from_this());
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
    for(size_t i = 0; i < children.size(); i++) {
        children[i]->resetScope();
    }
}
