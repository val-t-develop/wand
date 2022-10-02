#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/VarDeclNode.hpp>

class ForEachNode : public StatementNode {
public:
    shared_ptr<VarDeclNode> var;
    shared_ptr<ExpressionNode> expression;
    shared_ptr<StatementNode> statement;

    ForEachNode(shared_ptr<VarDeclNode> _var, shared_ptr<ExpressionNode> _expression,
                shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent);
};
