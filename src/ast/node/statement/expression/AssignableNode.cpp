#include "AssignableNode.hpp"


AssignableNode::AssignableNode(shared_ptr<Node> _parent) : ExpressionNode(_parent) {}

shared_ptr<ClassRecord> AssignableNode::getReturnType() {
    return nullptr;
}
