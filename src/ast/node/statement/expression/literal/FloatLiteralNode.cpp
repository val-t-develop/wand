#include "FloatLiteralNode.hpp"


FloatLiteralNode::FloatLiteralNode(double _value, bool _doubleVal, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), value(_value), doubleVal(_doubleVal) {}

shared_ptr<ClassRecord> FloatLiteralNode::getReturnType() {
    return nullptr;
}
