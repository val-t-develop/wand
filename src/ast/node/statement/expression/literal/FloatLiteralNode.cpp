#include "FloatLiteralNode.hpp"


FloatLiteralNode::FloatLiteralNode(long double _value, bool _doubleVal, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::FLOAT_LITERAL_NODE), value(_value), doubleVal(_doubleVal) {}

shared_ptr<ClassRecord> FloatLiteralNode::getReturnType() {
    return nullptr;
}
