#pragma once
#include "MethodRecord.hpp"
#include "Record.hpp"
#include "VarRecord.hpp"

class ClassRecord : public Record {
  public:
    vector<shared_ptr<MethodRecord>> methods =
        vector<shared_ptr<MethodRecord>>();
    vector<shared_ptr<VarRecord>> fields = vector<shared_ptr<VarRecord>>();
    vector<shared_ptr<ClassRecord>> innerClasses =
        vector<shared_ptr<ClassRecord>>();
    string superClass = "";

    ClassRecord(string _id, string _type);

    bool equals(shared_ptr<ClassRecord> r);

    void addMethod(shared_ptr<MethodRecord> method);
    void addField(shared_ptr<VarRecord> field);
    void addInnerClass(shared_ptr<ClassRecord> innerClass);

    shared_ptr<MethodRecord> getMethod(string name);
    shared_ptr<VarRecord> getField(string name);
    shared_ptr<ClassRecord> getInnerClass(string name);
    shared_ptr<Record> get(string name);

    string getFullName();
};
