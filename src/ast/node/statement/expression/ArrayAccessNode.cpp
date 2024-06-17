#include "ArrayAccessNode.hpp"

ArrayAccessNode::ArrayAccessNode(shared_ptr<ExpressionNode> _array,
                                 vector<shared_ptr<ExpressionNode>> _indexes,
                                 shared_ptr<Node> _parent)
    : AssignableNode(_parent, NodeKind::ARRAY_ACCESS_NODE), array(_array),
      indexes(_indexes) {}

shared_ptr<ClassRecord> ArrayAccessNode::getReturnType() {
    return array->getReturnType();
}