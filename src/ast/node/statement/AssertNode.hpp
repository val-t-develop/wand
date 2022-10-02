#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class AssertNode : public StatementNode {
public:
    shared_ptr<ExpressionNode> condition;
    shared_ptr<ExpressionNode> expression;

    AssertNode(shared_ptr<ExpressionNode> _condition, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent);
};
