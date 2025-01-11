/*  WAND - Wand Programming Language compiler
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Symbol of variable.
 */

#pragma once
#include "ClassRecord.hpp"
#include "Record.hpp"
#include <ast/node/ModifiersNode.hpp>

class VarRecord : public Record {
  public:
    shared_ptr<ClassRecord> typeRec;
    vector<ModifiersNode::ModifierKind> mods =
        vector<ModifiersNode::ModifierKind>();

    VarRecord(string _id, string _type, RecordKind _kind);

    bool equals(shared_ptr<VarRecord> r);

    string getFullName();
};
