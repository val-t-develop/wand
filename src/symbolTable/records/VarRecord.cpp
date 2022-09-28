#include "VarRecord.hpp"

VarRecord::VarRecord(string id, string type) : Record(id, type) {}


bool VarRecord::equals(shared_ptr<VarRecord> r) {
    return id == r->id && type == r->type && (next == nullptr ? false : next->equals(r->next));
}