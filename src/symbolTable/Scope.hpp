/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022-2024  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  Part of structure where all symbols (variables, methods, classes) in code are remembered. It represents function, class, or block of code in {}
 */

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
