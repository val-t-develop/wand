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
 *  Symbol of class.
 */

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
    shared_ptr<ClassRecord> superClass = nullptr;

    ClassRecord(string _id, string _type);

    bool equals(shared_ptr<ClassRecord> r);

    void addMethod(shared_ptr<MethodRecord> method);
    void addField(shared_ptr<VarRecord> field);
    void addInnerClass(shared_ptr<ClassRecord> innerClass);

    shared_ptr<MethodRecord> getMethod(string name);
    shared_ptr<VarRecord> getField(string name);
    shared_ptr<ClassRecord> getInnerClass(string name);
    shared_ptr<Record> get(string name);
    static shared_ptr<VarRecord> getSuperField(shared_ptr<ClassRecord> _this);

    string getFullName();
};
