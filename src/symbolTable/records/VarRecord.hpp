#pragma once
#include "ClassRecord.hpp"
#include "Record.hpp"
#include <ast/node/ModifiersNode.hpp>

class VarRecord : public Record {
  public:
    shared_ptr<ClassRecord> typeRec;
    vector<ModifiersNode::ModifierKind> mods =
        vector<ModifiersNode::ModifierKind>();

    VarRecord(string _id, string _type, RecordKind _kind);

    bool equals(shared_ptr<VarRecord> r);

    string getFullName();
};
