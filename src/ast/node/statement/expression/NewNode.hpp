#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/AccessNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>

class NewNode : public ExpressionNode {
public:
    shared_ptr<TypeNode> type;
    vector<shared_ptr<ExpressionNode>> args = vector<shared_ptr<ExpressionNode>>();

    NewNode(shared_ptr<TypeNode> _type, vector<shared_ptr<ExpressionNode>> _args, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};
