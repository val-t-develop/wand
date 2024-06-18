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
 *  Node of calling of the method.
 */

#include "MethodCallNode.hpp"

MethodCallNode::MethodCallNode(shared_ptr<MethodRecord> _record,
                               vector<shared_ptr<ExpressionNode>> _args,
                               shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::METHOD_CALL_NODE), record(_record),
      args(_args) {}

shared_ptr<ClassRecord> MethodCallNode::getReturnType() {
    return record->retTypeRec;
}

string MethodCallNode::getFullName() { return record->getFullName(); }
