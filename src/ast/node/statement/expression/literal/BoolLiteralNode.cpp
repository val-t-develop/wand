#include "BoolLiteralNode.hpp"

BoolLiteralNode::BoolLiteralNode(bool _value, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::BOOL_LITERAL_NODE), value(_value) {}

shared_ptr<ClassRecord> BoolLiteralNode::getReturnType() { return nullptr; }
