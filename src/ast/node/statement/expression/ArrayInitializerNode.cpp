#include "ArrayInitializerNode.hpp"

ArrayInitializerNode::ArrayInitializerNode(vector<shared_ptr<ExpressionNode>> _nodes, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::ARRAY_INITIALIZER_NODE), nodes(_nodes) {}

shared_ptr<ClassRecord> ArrayInitializerNode::getReturnType() {
    return nullptr;
}
