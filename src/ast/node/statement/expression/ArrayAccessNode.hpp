#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/AssignableNode.hpp>

class ArrayAccessNode : public AssignableNode {
public:
    shared_ptr<ExpressionNode> array;
    vector<shared_ptr<ExpressionNode>> indexes;

    ArrayAccessNode(shared_ptr<ExpressionNode> _array, vector<shared_ptr<ExpressionNode>> _indexes, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};

