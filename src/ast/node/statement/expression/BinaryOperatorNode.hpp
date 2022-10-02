#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class BinaryOperatorNode : public ExpressionNode {
public:
    enum class BinaryOperatorKind {
        RIGHT_SHIFT_ASSIGN, LEFT_SHIFT_ASSIGN,

        BIT_OR_ASSIGN, XOR_ASSIGN, MOD_ASSIGN, BIT_AND_ASSIGN,
        DIV_ASSIGN, MUL_ASSIGN, SUB_ASSIGN, ADD_ASSIGN, ASSIGN,

        OR, AND, BIT_OR, XOR, BIT_AND,
        EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, INSTANCEOF,
        LEFT_SHIFT, RIGHT_SHIFT,
        ADD, SUB, MUL, DIV, MOD
    };

    const static map<BinaryOperatorKind, string> operatorKinds;

    shared_ptr<ExpressionNode> left;
    shared_ptr<ExpressionNode> right;
    BinaryOperatorKind op;

    BinaryOperatorNode(shared_ptr<ExpressionNode> _left, shared_ptr<ExpressionNode> _right,
                       BinaryOperatorKind _op, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() ;

};
