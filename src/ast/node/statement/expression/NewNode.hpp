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
 *  Node of new operation.
 */

#pragma once
#include <Defs.hpp>
#include <ast/node/AccessNode.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>

class NewNode : public ExpressionNode {
  public:
    shared_ptr<TypeNode> type;
    vector<shared_ptr<ExpressionNode>> args =
        vector<shared_ptr<ExpressionNode>>();
    bool isStatic;

    NewNode(shared_ptr<TypeNode> _type,
            vector<shared_ptr<ExpressionNode>> _args, bool _isStatic,
            shared_ptr<Node> _parent, int _line, int _col);
    shared_ptr<ClassRecord> getReturnType() override;
};
