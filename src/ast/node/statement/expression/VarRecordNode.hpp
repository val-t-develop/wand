#pragma once
#include <Defs.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class VarRecordNode : public ExpressionNode {
public:
    shared_ptr<VarRecord> record;

    VarRecordNode(shared_ptr<VarRecord> _record, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;

    string getFullName();
};
