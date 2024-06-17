#pragma once
#include "records/ClassRecord.hpp"
#include "records/MethodRecord.hpp"
#include "records/Record.hpp"
#include "records/VarRecord.hpp"
#include <Defs.hpp>

class Scope : public std::enable_shared_from_this<Scope> {
  public:
    size_t next = 0;
    shared_ptr<Scope> parent = nullptr;
    vector<shared_ptr<Scope>> children = vector<shared_ptr<Scope>>();

    vector<shared_ptr<VarRecord>> varRecords = vector<shared_ptr<VarRecord>>();
    vector<shared_ptr<MethodRecord>> methodRecords =
        vector<shared_ptr<MethodRecord>>();
    vector<shared_ptr<ClassRecord>> classRecords =
        vector<shared_ptr<ClassRecord>>();
    vector<shared_ptr<Record>> records = vector<shared_ptr<Record>>();

    shared_ptr<ClassRecord> containingClass =
        make_shared<ClassRecord>("__program", "__program");

    shared_ptr<Record> record;

    string scopeName = "";
    string scopeType = "";

    Scope(shared_ptr<Scope> parent);
    Scope();

    void setScopeNameAndType(string name, string type);

    shared_ptr<VarRecord> lookupVar(const string &name);
    shared_ptr<MethodRecord> lookupMethod(const string &name);
    shared_ptr<ClassRecord> lookupClass(const string &name);
    shared_ptr<Record> lookupRecord(const string &name);
    shared_ptr<Record> lookup(const string &name);

    void put(shared_ptr<VarRecord> varRecord);
    void put(shared_ptr<MethodRecord> methodRecord);
    void put(shared_ptr<ClassRecord> classRecord);
    void put(shared_ptr<Record> record);

    shared_ptr<Scope> nextChild(shared_ptr<Record> record);

    void resetScope();

    int getIdByKeyVar(const string &key);
    int getIdByKeyMethod(const string &key);
    int getIdByKeyClass(const string &key);
    int getIdByKeyRecord(const string &key);
};
