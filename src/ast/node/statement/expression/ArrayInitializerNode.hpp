#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class ArrayInitializerNode : public ExpressionNode {
  public:
    vector<shared_ptr<ExpressionNode>> nodes =
        vector<shared_ptr<ExpressionNode>>();

    ArrayInitializerNode(vector<shared_ptr<ExpressionNode>> _nodes,
                         shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};
