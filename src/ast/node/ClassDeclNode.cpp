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
 *  Node of class declaration.
 */

#include "ClassDeclNode.hpp"

ClassDeclNode::ClassDeclNode(
    shared_ptr<GenericNode> _generic, shared_ptr<ModifiersNode> _modifiers,
    ClassKind _kind, shared_ptr<ClassRecord> _record,
    shared_ptr<TypeNode> _extended,
    vector<shared_ptr<TypeNode>> _implemented,
    vector<shared_ptr<VarDeclNode>> _fields,
    vector<shared_ptr<MethodDeclNode>> _methods,
    vector<shared_ptr<ConstructorDeclNode>> _constructors,
    shared_ptr<DestructorDeclNode> _destructor,
    vector<shared_ptr<ClassDeclNode>> _innerClasses, shared_ptr<Node> _parent, int _l, int _c)
    : Node(_parent, NodeKind::CLASS_DECL_NODE), generic(_generic),
      modifiers(_modifiers), kind(_kind), record(_record), extended(_extended),
      implemented(_implemented), fields(_fields), methods(_methods),
      constructors(_constructors), destructor(_destructor), innerClasses(_innerClasses) {
    setPos(_l, _c);
}

string ClassDeclNode::getFullName() { return record->getFullName(); }
