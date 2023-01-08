#include "VarRecord.hpp"

VarRecord::VarRecord(string _id, string _type, RecordKind _kind) : Record(_id, _type, _kind) {}


bool VarRecord::equals(shared_ptr<VarRecord> r) {
    return id == r->id && type == r->type && (next == nullptr ? r->next == nullptr : next->equals(r->next));
}
