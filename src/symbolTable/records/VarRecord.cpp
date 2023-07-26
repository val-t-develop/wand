#include "VarRecord.hpp"
#include <utils/Out.hpp>

VarRecord::VarRecord(string _id, string _type, RecordKind _kind) : Record(_id, _type, _kind) {}


bool VarRecord::equals(shared_ptr<VarRecord> r) {
    return id == r->id && type == r->type && (next == nullptr ? r->next == nullptr : next->equals(r->next));
}

string VarRecord::getFullName() {
    if (ir_name == "") {
        Out::errorMessage("Internal error detected! Can not get ir_name of var or field " + id);
    }
    return ir_name;
}
