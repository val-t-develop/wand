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

#include "BinaryOperatorNode.hpp"

const map<string, BinaryOperatorNode::BinaryOperatorKind>
    BinaryOperatorNode::operatorKinds = {
        {">>=", BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT_ASSIGN},
        {"<<=", BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT_ASSIGN},
        {"|=", BinaryOperatorNode::BinaryOperatorKind::BIT_OR_ASSIGN},
        {"^=", BinaryOperatorNode::BinaryOperatorKind::XOR_ASSIGN},
        {"%=", BinaryOperatorNode::BinaryOperatorKind::MOD_ASSIGN},
        {"&=", BinaryOperatorNode::BinaryOperatorKind::BIT_AND_ASSIGN},
        {"/=", BinaryOperatorNode::BinaryOperatorKind::DIV_ASSIGN},
        {"*=", BinaryOperatorNode::BinaryOperatorKind::MUL_ASSIGN},
        {"-=", BinaryOperatorNode::BinaryOperatorKind::SUB_ASSIGN},
        {"+=", BinaryOperatorNode::BinaryOperatorKind::ADD_ASSIGN},
        {"=", BinaryOperatorNode::BinaryOperatorKind::ASSIGN},
        {"||", BinaryOperatorNode::BinaryOperatorKind::OR},
        {"&&", BinaryOperatorNode::BinaryOperatorKind::AND},
        {"|", BinaryOperatorNode::BinaryOperatorKind::BIT_OR},
        {"^", BinaryOperatorNode::BinaryOperatorKind::XOR},
        {"&", BinaryOperatorNode::BinaryOperatorKind::BIT_AND},
        {"==", BinaryOperatorNode::BinaryOperatorKind::EQUAL},
        {"!=", BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL},
        {"<", BinaryOperatorNode::BinaryOperatorKind::LESS},
        {">", BinaryOperatorNode::BinaryOperatorKind::GREATER},
        {"<=", BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL},
        {">=", BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL},
        {"instanceof", BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF},
        {"<<", BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT},
        {">>", BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT},
        {"+", BinaryOperatorNode::BinaryOperatorKind::ADD},
        {"-", BinaryOperatorNode::BinaryOperatorKind::SUB},
        {"*", BinaryOperatorNode::BinaryOperatorKind::MUL},
        {"/", BinaryOperatorNode::BinaryOperatorKind::DIV},
        {"%", BinaryOperatorNode::BinaryOperatorKind::MOD}};

BinaryOperatorNode::BinaryOperatorNode(shared_ptr<ExpressionNode> _left,
                                       shared_ptr<ExpressionNode> _right,
                                       BinaryOperatorKind _op,
                                       shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::BINARY_OPERATOR_NODE), left(_left),
      right(_right), op(_op) {}

shared_ptr<ClassRecord> BinaryOperatorNode::getReturnType() {
    if (op==BinaryOperatorKind::RIGHT_SHIFT_ASSIGN ||
        op==BinaryOperatorKind::LEFT_SHIFT_ASSIGN ||
        op==BinaryOperatorKind::BIT_OR_ASSIGN ||
        op==BinaryOperatorKind::XOR_ASSIGN ||
        op==BinaryOperatorKind::MOD_ASSIGN ||
        op==BinaryOperatorKind::BIT_AND_ASSIGN ||
        op==BinaryOperatorKind::DIV_ASSIGN ||
        op==BinaryOperatorKind::MUL_ASSIGN ||
        op==BinaryOperatorKind::SUB_ASSIGN ||
        op==BinaryOperatorKind::ADD_ASSIGN ||
        op==BinaryOperatorKind::ASSIGN ||
        op==BinaryOperatorKind::BIT_OR ||
        op==BinaryOperatorKind::BIT_AND ||
        op==BinaryOperatorKind::LEFT_SHIFT ||
        op==BinaryOperatorKind::RIGHT_SHIFT) {
        return left->getReturnType();
    } else if (op==BinaryOperatorKind::OR ||
        op==BinaryOperatorKind::AND ||
        op==BinaryOperatorKind::LESS ||
        op==BinaryOperatorKind::GREATER ||
        op==BinaryOperatorKind::LESS_EQUAL ||
        op==BinaryOperatorKind::GREATER_EQUAL ||
        op==BinaryOperatorKind::EQUAL ||
        op==BinaryOperatorKind::NOT_EQUAL ||
        op==BinaryOperatorKind::INSTANCEOF) {
        return make_shared<ClassRecord>("bool", "primitive");
    } else if (op==BinaryOperatorKind::ADD && left->getReturnType()->getFullName()=="String") {
        return left->getReturnType();
    } else if (op==BinaryOperatorKind::ADD && right->getReturnType()->getFullName()=="String") {
        return right->getReturnType();
    } else if (op==BinaryOperatorKind::ADD ||
        op==BinaryOperatorKind::SUB ||
        op==BinaryOperatorKind::MUL ||
        op==BinaryOperatorKind::DIV) {
        if (left->getReturnType()->getFullName()=="double") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="double") {
            return right->getReturnType();
        } else if (left->getReturnType()->getFullName()=="float") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="float") {
            return right->getReturnType();
        } else if (left->getReturnType()->getFullName()=="long") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="long") {
            return right->getReturnType();
        } else if (left->getReturnType()->getFullName()=="int") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="int") {
            return right->getReturnType();
        } if (left->getReturnType()->getFullName()=="short") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="short") {
            return right->getReturnType();
        } else if (left->getReturnType()->getFullName()=="byte") {
            return left->getReturnType();
        } else if (right->getReturnType()->getFullName()=="byte") {
            return right->getReturnType();
        }
    }
}
