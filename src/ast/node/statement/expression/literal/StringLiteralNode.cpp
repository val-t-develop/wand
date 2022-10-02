#include "StringLiteralNode.hpp"


StringLiteralNode::StringLiteralNode(string _str, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), str(_str) {}

shared_ptr<ClassRecord> StringLiteralNode::getReturnType() {
    return nullptr;
}
