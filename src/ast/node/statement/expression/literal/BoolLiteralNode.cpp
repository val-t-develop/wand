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
 *  Node of bool-type literal.
 */

#include "BoolLiteralNode.hpp"

BoolLiteralNode::BoolLiteralNode(bool _value, shared_ptr<ClassRecord> _typeRec, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::BOOL_LITERAL_NODE), typeRec(_typeRec), value(_value) {}

shared_ptr<ClassRecord> BoolLiteralNode::getReturnType() { return typeRec; }
