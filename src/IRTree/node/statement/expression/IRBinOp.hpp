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
 *  IR tree part for binary operation.
 */

#pragma once
#include "IRExpression.hpp"

#include <ast/node/statement/expression/BinaryOperatorNode.hpp>

class IRBinOp : public IRExpression {
public:
    shared_ptr<IRExpression> left;
    shared_ptr<IRExpression> right;
    BinaryOperatorNode::BinaryOperatorKind opKind;

    IRBinOp(shared_ptr<IRExpression> _left, shared_ptr<IRExpression> _right, BinaryOperatorNode::BinaryOperatorKind _opKind);
    string getReturnType(shared_ptr<CodeGen> codeGen) override;
};
