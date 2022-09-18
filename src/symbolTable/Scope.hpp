#pragma once
#include <Defs.hpp>
#include "records/Record.hpp"
#include "records/VarRecord.hpp"
#include "records/MethodRecord.hpp"
#include "records/ClassRecord.hpp"

class Scope {
public:
  size_t next;
  shared_ptr<Scope> parent;
  vector<shared_ptr<Scope>> children = vector<shared_ptr<Scope>>();

  vector<shared_ptr<VarRecord>> varRecords = vector<shared_ptr<VarRecord>>();
  vector<shared_ptr<MethodRecord>> methodRecords = vector<shared_ptr<MethodRecord>>();
  vector<shared_ptr<ClassRecord>> classRecords = vector<shared_ptr<ClassRecord>>();

  shared_ptr<ClassRecord> containingClass = make_shared<ClassRecord>("__program", "__program");

  shared_ptr<Record> record;

  string scopeName = "";
  string scopeType = "";

  Scope(shared_ptr<Scope> parent);
  Scope(Scope* r);

  void setScopeNameAndType(string name, string type);

  shared_ptr<VarRecord> lookupVar(string name);
  shared_ptr<MethodRecord> lookupMethod(string name);
  shared_ptr<ClassRecord> lookupClass(string name);
  shared_ptr<Record> lookup(string name);

  void put(shared_ptr<VarRecord> varRecord);
  void put(shared_ptr<MethodRecord> methodRecord);
  void put(shared_ptr<ClassRecord> classRecord);

  shared_ptr<Scope> nextChild(shared_ptr<Record> record);

  void resetScope();

  static int getIdByKey(vector<shared_ptr<VarRecord>> list, string key);
  static int getIdByKey(vector<shared_ptr<MethodRecord>> list, string key);
  static int getIdByKey(vector<shared_ptr<ClassRecord>> list, string key);
};