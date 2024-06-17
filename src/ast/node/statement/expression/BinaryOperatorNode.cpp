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

shared_ptr<ClassRecord> BinaryOperatorNode::getReturnType() { return nullptr; }
