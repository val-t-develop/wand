#include "Record.hpp"

Record::Record(string _id, string _type, RecordKind _kind) {
    this->id = _id;
    this->type = _type;
    this->kind = _kind;
}

bool Record::equals(shared_ptr<Record> r) {
    return id == r->id && type == r->type && (next == nullptr ? r->next == nullptr : next->equals(r->next));
}
