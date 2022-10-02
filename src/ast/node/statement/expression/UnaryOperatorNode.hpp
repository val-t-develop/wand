#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class UnaryOperatorNode : public ExpressionNode {
public:
    bool isPre;
    string op;
    shared_ptr<ExpressionNode> expression;

    UnaryOperatorNode(bool _isPre, string _op, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() ;
};
