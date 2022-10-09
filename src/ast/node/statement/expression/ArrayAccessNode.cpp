#include "ArrayAccessNode.hpp"

ArrayAccessNode::ArrayAccessNode(shared_ptr<ExpressionNode> _array, shared_ptr<ExpressionNode> _index, shared_ptr<Node> _parent)
    : AssignableNode(_parent, NodeKind::ARRAY_ACCESS_NODE), array(_array), index(_index) {}

shared_ptr<ClassRecord> ArrayAccessNode::getReturnType() {
    return nullptr;
}
