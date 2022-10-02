#pragma once
#include <Defs.hpp>
#include <ast/node/statement/StatementNode.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class WhileNode : public StatementNode {
public:
    shared_ptr<ExpressionNode> expression;
    shared_ptr<StatementNode> statement;

    WhileNode(shared_ptr<ExpressionNode> _expression, shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent);
};
