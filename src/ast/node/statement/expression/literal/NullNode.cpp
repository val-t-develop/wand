#include "NullNode.hpp"


NullLiteralNode::NullLiteralNode(shared_ptr<Node> _parent) : ExpressionNode(_parent) {}

shared_ptr<ClassRecord> NullLiteralNode::getReturnType() {
    return nullptr;
}
