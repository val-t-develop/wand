#include "NullNode.hpp"


NullLiteralNode::NullLiteralNode(shared_ptr<Node> _parent) : ExpressionNode(_parent, NodeKind::NULL_LITERAL_NODE) {}

shared_ptr<ClassRecord> NullLiteralNode::getReturnType() {
    return nullptr;
}
