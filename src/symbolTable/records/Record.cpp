#include "Record.hpp"

Record::Record(string id, string type) {
    this->id = id;
    this->type = type;
}

bool Record::equals(shared_ptr<Record> r) {
    return id == r->id && type == r->type && (next == nullptr ? false : next->equals(r->next));
}
