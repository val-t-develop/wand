#include "StringLiteralNode.hpp"

StringLiteralNode::StringLiteralNode(string _str, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::STRING_LITERAL_NODE), str(_str) {}

shared_ptr<ClassRecord> StringLiteralNode::getReturnType() { return nullptr; }
