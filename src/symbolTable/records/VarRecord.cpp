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

#include "VarRecord.hpp"
#include <utils/Out.hpp>

VarRecord::VarRecord(string _id, string _type, RecordKind _kind)
    : Record(_id, _type, _kind) {}

bool VarRecord::equals(shared_ptr<VarRecord> r) {
    return id == r->id && type == r->type &&
           (next == nullptr ? r->next == nullptr : next->equals(r->next));
}

string VarRecord::getFullName() {
    if (ir_name == "") {
        Out::errorMessage(
            "Internal error detected! Can not get ir_name of var or field " +
            id);
    }
    return ir_name;
}
