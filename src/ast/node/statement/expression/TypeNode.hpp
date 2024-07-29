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
 *  Node of type specifier.
 */

#pragma once
#include <Defs.hpp>
#include <ast/node/AccessNode.hpp>
#include <ast/node/ClassRecordNode.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class TypeNode : public ExpressionNode {
  public:
    shared_ptr<ClassRecordNode> type;
    int dims;

    TypeNode(shared_ptr<ClassRecordNode> _type, int _dims,
             shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;

    string getFullName();
};
