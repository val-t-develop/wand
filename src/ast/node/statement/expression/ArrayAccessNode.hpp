#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/AssignableNode.hpp>

class ArrayAccessNode : public AssignableNode {
public:
    shared_ptr<ExpressionNode> array;
    shared_ptr<ExpressionNode> index;

    ArrayAccessNode(shared_ptr<ExpressionNode> _array, shared_ptr<ExpressionNode> _index, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() ;
};

