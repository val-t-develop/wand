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
 *  Node of destructor declaration.
 */

#include "DestructorDeclNode.hpp"

DestructorDeclNode::DestructorDeclNode(shared_ptr<ModifiersNode> _modifiers,
                                         shared_ptr<MethodRecord> _record,
                                         shared_ptr<BlockNode> _body,
                                         shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::DESTRUCTOR_DECL_NODE), modifiers(_modifiers),
      record(_record), body(_body) {
    // TODO
}
