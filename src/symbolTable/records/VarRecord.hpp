#pragma once
#include "Record.hpp"
#include "ClassRecord.hpp"

class VarRecord : public Record {
public:
    shared_ptr<ClassRecord> typeRec;

    VarRecord(string _id, string _type, RecordKind _kind);

    bool equals(shared_ptr<VarRecord> r);
};
