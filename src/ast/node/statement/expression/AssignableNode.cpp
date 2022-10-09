#include "AssignableNode.hpp"


AssignableNode::AssignableNode(shared_ptr<Node> _parent, NodeKind _kind) : ExpressionNode(_parent, _kind) {}

shared_ptr<ClassRecord> AssignableNode::getReturnType() {
    return nullptr;
}
