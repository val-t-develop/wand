#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class VarRecordNode : public ExpressionNode {
  public:
    shared_ptr<VarRecord> record;

    VarRecordNode(shared_ptr<VarRecord> _record, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;

    string getFullName();
};
