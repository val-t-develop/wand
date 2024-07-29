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
 *  Node of binary operator.
 */

#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class BinaryOperatorNode : public ExpressionNode {
  public:
    enum class BinaryOperatorKind {
        RIGHT_SHIFT_ASSIGN,
        LEFT_SHIFT_ASSIGN,

        BIT_OR_ASSIGN,
        XOR_ASSIGN,
        MOD_ASSIGN,
        BIT_AND_ASSIGN,
        DIV_ASSIGN,
        MUL_ASSIGN,
        SUB_ASSIGN,
        ADD_ASSIGN,
        ASSIGN,

        OR,
        AND,
        BIT_OR,
        XOR,
        BIT_AND,
        EQUAL,
        NOT_EQUAL,
        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL,
        INSTANCEOF,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD
    };

    const static map<string, BinaryOperatorKind> operatorKinds;

    shared_ptr<ExpressionNode> left;
    shared_ptr<ExpressionNode> right;
    BinaryOperatorKind op;

    BinaryOperatorNode(shared_ptr<ExpressionNode> _left,
                       shared_ptr<ExpressionNode> _right,
                       BinaryOperatorKind _op, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;
};
