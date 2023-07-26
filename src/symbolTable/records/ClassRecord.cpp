#include "ClassRecord.hpp"

ClassRecord::ClassRecord(string _id, string _type) : Record(_id, _type, RecordKind::CLASS_RECORD) {}

bool ClassRecord::equals(shared_ptr<ClassRecord> r) {
    bool isComponentsEqual;

    if(methods.size() == r->methods.size()) {
        for(size_t i = 0; i < methods.size(); ++i) {
            if(!methods[i]->equals(r->methods[i])) {
                isComponentsEqual = false;
            }
        }
    }

    if(fields.size() == r->fields.size()) {
        for(size_t i = 0; i < fields.size(); ++i) {
            if(!fields[i]->equals(r->fields[i])) {
                isComponentsEqual = false;
            }
        }
    }

    if(innerClasses.size() == r->innerClasses.size()) {
        for(size_t i = 0; i < innerClasses.size(); ++i) {
            if(!innerClasses[i]->equals(r->innerClasses[i])) {
                isComponentsEqual = false;
            }
        }
    }

    return id == r->id && type == r->type && (next == nullptr ? r->next == nullptr : next->equals(r->next)) &&
           superClass == r->superClass && isComponentsEqual;
}

void ClassRecord::addMethod(shared_ptr<MethodRecord> method) {
    methods.push_back(method);
}

void ClassRecord::addField(shared_ptr<VarRecord> field) {
    fields.push_back(field);
}

void ClassRecord::addInnerClass(shared_ptr<ClassRecord> innerClass) {
    innerClasses.push_back(innerClass);
}

shared_ptr<MethodRecord> ClassRecord::getMethod(string name) {
    for(shared_ptr<MethodRecord> method : methods) {
        if(method->id == name) {
            return method;
        }
    }
    return nullptr;
}

shared_ptr<VarRecord> ClassRecord::getField(string name) {
    for(shared_ptr<VarRecord> field : fields) {
        if(field->id == name) {
            return field;
        }
    }
    return nullptr;
}

shared_ptr<ClassRecord> ClassRecord::getInnerClass(string name) {
    for(shared_ptr<ClassRecord> innerClass : innerClasses) {
        if(innerClass->id == name) {
            return innerClass;
        }
    }
    return nullptr;
}

shared_ptr<Record> ClassRecord::get(string name) {
    shared_ptr<VarRecord> field = getField(name);
    shared_ptr<MethodRecord> method = getMethod(name);
    shared_ptr<ClassRecord> innerClass = getInnerClass(name);

    if(field != nullptr) {
        return field;
    } else if(method != nullptr) {
        return method;
    } else {
        return innerClass;
    }
}

string ClassRecord::getFullName() {
    if (ir_name == "") {
        string str = "";
        if (next != nullptr) {
            str += static_pointer_cast<ClassRecord>(next)->getFullName() + ".";
        }
        str += id;
        ir_name = str;
    }
    return ir_name;
}
