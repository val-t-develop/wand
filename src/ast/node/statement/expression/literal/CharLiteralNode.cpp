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
 *  Node of char-type literal.
 */

#include "CharLiteralNode.hpp"

CharLiteralNode::CharLiteralNode(string _str, shared_ptr<ClassRecord> _typeRec, shared_ptr<Node> _parent, int _line, int _col)
    : ExpressionNode(_parent, NodeKind::CHAR_LITERAL_NODE, _line, _col), typeRec(_typeRec), str(_str) {}

shared_ptr<ClassRecord> CharLiteralNode::getReturnType() { return typeRec; }
