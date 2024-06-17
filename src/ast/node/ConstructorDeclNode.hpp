#pragma once
#include <Defs.hpp>
#include <ast/node/VarDeclNode.hpp>
#include <ast/node/statement/BlockNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class ConstructorDeclNode : public Node {
  public:
    shared_ptr<ModifiersNode> modifiers;
    shared_ptr<MethodRecord> record; // For ir_name storage
    vector<shared_ptr<VarDeclNode>> args;
    shared_ptr<BlockNode> body;

    ConstructorDeclNode(shared_ptr<ModifiersNode> _modifiers,
                        shared_ptr<MethodRecord> _record,
                        vector<shared_ptr<VarDeclNode>> _args,
                        shared_ptr<BlockNode> _body, shared_ptr<Node> _parent);
};
