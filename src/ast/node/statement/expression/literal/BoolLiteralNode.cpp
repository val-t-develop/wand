#include "BoolLiteralNode.hpp"


BoolLiteralNode::BoolLiteralNode(bool _value, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), value(_value) {}


shared_ptr<ClassRecord> BoolLiteralNode::getReturnType() {
    return nullptr;
}
