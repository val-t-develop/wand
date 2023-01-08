#pragma once
#include <Defs.hpp>

class Record {
public:

    enum class RecordKind {
        UNUSED,
        METHOD_RECORD,
        LOCAL_VAR_RECORD,
        FIELD_RECORD, 
        CLASS_RECORD
    };


    string id;
    string type;
    shared_ptr<Record> next;
    string ir_name = "";
    RecordKind kind;

    Record(string _id, string _type, RecordKind _kind);

    bool equals(shared_ptr<Record> r);
};

class VarRecord;
class MethodRecord;
class ClassRecord;