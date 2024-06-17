#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class IfElseNode : public StatementNode {
  public:
    shared_ptr<ExpressionNode> condition;
    shared_ptr<StatementNode> thenNode;
    shared_ptr<StatementNode> elseNode;

    IfElseNode(shared_ptr<ExpressionNode> _condition,
               shared_ptr<StatementNode> _thenNode,
               shared_ptr<StatementNode> _elseNode, shared_ptr<Node> _parent);
};
