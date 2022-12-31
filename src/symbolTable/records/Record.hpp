#pragma once
#include <Defs.hpp>

class Record {
public:
    string id;
    string type;
    shared_ptr<Record> next;
    string ir_name = "";

    Record(string id, string type);

    bool equals(shared_ptr<Record> r);
};
