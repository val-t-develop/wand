#pragma once
#include <Defs.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <ast/node/ModifiersNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class VarDeclNode : public StatementNode {
public:
    shared_ptr<ModifiersNode> modifiers;
    shared_ptr<TypeNode> type;
    shared_ptr<VarRecord> record;
    shared_ptr<ExpressionNode> init;

    VarDeclNode(shared_ptr<ModifiersNode> _modifiers,
                shared_ptr<TypeNode> _type,
                shared_ptr<VarRecord> _record,
                shared_ptr<ExpressionNode> _init, shared_ptr<Node> _parent);
};
