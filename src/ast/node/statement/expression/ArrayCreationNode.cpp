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
 *  Node of creation an array.
 */

#include "ArrayCreationNode.hpp"

ArrayCreationNode::ArrayCreationNode(shared_ptr<TypeNode> _type,
                                     vector<shared_ptr<ExpressionNode>> _dims,
                                     shared_ptr<ArrayInitializerNode> _init,
                                     bool _isStatic, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::ARRAY_CREATION_NODE), type(_type),
      dims(_dims), init(_init), isStatic(_isStatic) {}

shared_ptr<ClassRecord> ArrayCreationNode::getReturnType() { return nullptr; }
