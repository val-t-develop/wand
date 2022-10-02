#pragma once
#include "Record.hpp"

class VarRecord : public Record {
public:
    VarRecord(string id, string type);

    bool equals(shared_ptr<VarRecord> r);
};
