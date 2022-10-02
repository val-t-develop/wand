#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class ReturnNode : public StatementNode {
public:
    shared_ptr<ExpressionNode> expression;

    ReturnNode(shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent);
};
