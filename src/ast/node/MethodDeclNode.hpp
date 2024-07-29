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
 *  Node of method declaration.
 */

#pragma once
#include <Defs.hpp>
#include <ast/node/VarDeclNode.hpp>
#include <ast/node/statement/BlockNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class MethodDeclNode : public Node {
  public:
    shared_ptr<ModifiersNode> modifiers;
    shared_ptr<TypeNode> returnType;
    shared_ptr<MethodRecord> record;
    vector<shared_ptr<VarDeclNode>> args;
    shared_ptr<BlockNode> body;

    MethodDeclNode(shared_ptr<ModifiersNode> _modifiers,
                   shared_ptr<TypeNode> _returnType,
                   shared_ptr<MethodRecord> _record,
                   vector<shared_ptr<VarDeclNode>> _args,
                   shared_ptr<BlockNode> _body, shared_ptr<Node> _parent);

    string getFullName();
};
