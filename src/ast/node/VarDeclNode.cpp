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
 *  Node of variable declaration.
 */

#include "VarDeclNode.hpp"

VarDeclNode::VarDeclNode(shared_ptr<ModifiersNode> _modifiers,
                         shared_ptr<TypeNode> _type,
                         shared_ptr<VarRecord> _record,
                         shared_ptr<ExpressionNode> _init,
                         shared_ptr<Node> _parent, int _l, int _c)
    : StatementNode(_parent, NodeKind::VAR_DECL_NODE), modifiers(_modifiers),
      type(_type), record(_record), init(_init) {
    setPos(_l, _c);
}

string VarDeclNode::getFullName() {
    if (record->ir_name == "") {
        string str = "";
        if (record->next != nullptr) {
            str +=
                static_pointer_cast<ClassRecord>(record->next)->getFullName() +
                ".";
        }
        str += record->id;

        str += "__wand__" + type->type->record->getFullName();
        record->ir_name = str;
    }
    return record->ir_name;
}
