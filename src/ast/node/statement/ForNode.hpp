#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class ForNode : public StatementNode {
public:
    shared_ptr<StatementNode> init;
    shared_ptr<ExpressionNode> condition;
    shared_ptr<StatementNode> update;
    shared_ptr<StatementNode> statement;

    ForNode(shared_ptr<StatementNode> _init, shared_ptr<ExpressionNode> _condition,
            shared_ptr<StatementNode> _update, shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent);
};
