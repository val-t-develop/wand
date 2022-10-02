#include "ArrayInitializerNode.hpp"

ArrayInitializerNode::ArrayInitializerNode(vector<shared_ptr<ExpressionNode>> _nodes, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), nodes(_nodes) {}

shared_ptr<ClassRecord> ArrayInitializerNode::getReturnType() {
    return nullptr;
}
