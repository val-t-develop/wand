#include "CharLiteralNode.hpp"

CharLiteralNode::CharLiteralNode(string _str, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::CHAR_LITERAL_NODE), str(_str) {}

shared_ptr<ClassRecord> CharLiteralNode::getReturnType() { return nullptr; }
