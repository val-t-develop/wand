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

#pragma once
#include <Defs.hpp>
#include <ast/node/ConstructorDeclNode.hpp>
#include <ast/node/DestructorDeclNode.hpp>
#include <ast/node/GenericNode.hpp>
#include <ast/node/MethodDeclNode.hpp>
#include <ast/node/ModifiersNode.hpp>
#include <ast/node/Node.hpp>
#include <ast/node/VarDeclNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class ClassDeclNode : public Node {
  public:
    enum class ClassKind { CLASS, INTERFACE, ENUM };
    shared_ptr<GenericNode> generic;
    shared_ptr<ModifiersNode> modifiers;
    ClassKind kind;
    shared_ptr<ClassRecord> record;
    shared_ptr<TypeNode> extended = shared_ptr<TypeNode>();
    vector<shared_ptr<TypeNode>> implemented = vector<shared_ptr<TypeNode>>();
    vector<shared_ptr<VarDeclNode>> fields = vector<shared_ptr<VarDeclNode>>();
    vector<shared_ptr<MethodDeclNode>> methods =
        vector<shared_ptr<MethodDeclNode>>();
    vector<shared_ptr<ConstructorDeclNode>> constructors =
        vector<shared_ptr<ConstructorDeclNode>>();
    shared_ptr<DestructorDeclNode> destructor = nullptr;
    vector<shared_ptr<ClassDeclNode>> innerClasses =
        vector<shared_ptr<ClassDeclNode>>();

    ClassDeclNode(shared_ptr<GenericNode> _generic,
                  shared_ptr<ModifiersNode> _modifiers, ClassKind _kind,
                  shared_ptr<ClassRecord> _record,
                  shared_ptr<TypeNode> _extended,
                  vector<shared_ptr<TypeNode>> _implemented,
                  vector<shared_ptr<VarDeclNode>> _fields,
                  vector<shared_ptr<MethodDeclNode>> _methods,
                  vector<shared_ptr<ConstructorDeclNode>> _constructors,
                  shared_ptr<DestructorDeclNode> _destructor,
                  vector<shared_ptr<ClassDeclNode>> _innerClasses,
                  shared_ptr<Node> _parent, int _l, int _c);

    string getFullName();
};
