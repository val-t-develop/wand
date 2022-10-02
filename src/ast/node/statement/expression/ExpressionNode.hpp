#pragma once
#include <Defs.hpp>
#include <ast/node/statement/StatementNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class ExpressionNode : public StatementNode {
public:
    ExpressionNode(shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType();
    ~ExpressionNode();
};
