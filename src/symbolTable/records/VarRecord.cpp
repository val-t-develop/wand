#include "VarRecord.hpp"

VarRecord::VarRecord(string id, string type) : Record(id, type) {}


bool VarRecord::equals(shared_ptr<VarRecord> r) {
    return id == r->id && type == r->type && (next == nullptr ? r->next == nullptr : next->equals(r->next));
}
