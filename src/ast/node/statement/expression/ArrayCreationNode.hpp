#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <ast/node/statement/expression/ArrayInitializerNode.hpp>

class ArrayCreationNode : public ExpressionNode {
public:
    shared_ptr<TypeNode> type;
    vector<shared_ptr<ExpressionNode>> dims = vector<shared_ptr<ExpressionNode>>();
    shared_ptr<ArrayInitializerNode> init;

    ArrayCreationNode(shared_ptr<TypeNode> _type, vector<shared_ptr<ExpressionNode>> _dims,
                      shared_ptr<ArrayInitializerNode> _init, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};
