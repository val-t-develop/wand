#pragma once
#include "Record.hpp"
#include "VarRecord.hpp"
#include "ClassRecord.hpp"
#include <ast/node/ModifiersNode.hpp>

class MethodRecord : public Record {
public:
    shared_ptr<ClassRecord> retTypeRec;
    vector<shared_ptr<VarRecord>> vars = vector<shared_ptr<VarRecord>>();
    int argsCount = 0;
    bool isConstructor = false;
    vector<shared_ptr<MethodRecord>> similar = vector<shared_ptr<MethodRecord>>();
    vector<ModifiersNode::ModifierKind> mods = vector<ModifiersNode::ModifierKind>();

    MethodRecord(string _id, string _type);

    bool equals(shared_ptr<MethodRecord> r);
    void addVar(shared_ptr<VarRecord> var);
    void addArg(shared_ptr<VarRecord> var);
    bool containArg(shared_ptr<VarRecord> var);
    bool containArg(shared_ptr<VarRecord> var, int n);

    string getFullName();
};
