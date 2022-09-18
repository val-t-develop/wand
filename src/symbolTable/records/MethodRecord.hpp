#pragma once
#include "Record.hpp"
#include "VarRecord.hpp"

class MethodRecord : public Record {
public:
    vector<shared_ptr<VarRecord>> vars = vector<shared_ptr<VarRecord>>();
    int argsCount = 0;
    bool isConstructor = false;

    MethodRecord(string id, string type);

    bool equals(shared_ptr<MethodRecord> r);
    void addVar(shared_ptr<VarRecord> var);
    void addArg(shared_ptr<VarRecord> var);
    bool containArg(shared_ptr<VarRecord> var);
    bool containArg(shared_ptr<VarRecord> var, int n);
};