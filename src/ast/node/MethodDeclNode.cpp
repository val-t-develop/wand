/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022  Valentyn Tymchyshyn
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
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  Node of method declaration.
 */

#include "MethodDeclNode.hpp"

#include "ClassDeclNode.hpp"

MethodDeclNode::MethodDeclNode(shared_ptr<ModifiersNode> _modifiers,
                               shared_ptr<TypeNode> _returnType,
                               shared_ptr<MethodRecord> _record,
                               vector<shared_ptr<VarDeclNode>> _args,
                               shared_ptr<BlockNode> _body,
                               shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::METHOD_DECL_NODE), modifiers(_modifiers),
      returnType(_returnType), record(_record), args(_args), body(_body) {
    // TODO
}

string MethodDeclNode::getFullName() {
    if (record->ir_name == "") {
        string str = "";
        if (record->next != nullptr) {
            str +=
                static_pointer_cast<ClassRecord>(record->next)->getFullName() +
                ".";
        }
        str += record->id;

        str += "__spl__" + returnType->type->record->getFullName();
        for (auto arg : args) {
            if (arg->record->id=="this") {
                str += "__" + static_pointer_cast<ClassDeclNode>(parent)->getFullName();
            } else {
                str += "__" + arg->type->type->record->getFullName();
            }
        }
        record->ir_name = str;
    }
    return record->ir_name;
}
