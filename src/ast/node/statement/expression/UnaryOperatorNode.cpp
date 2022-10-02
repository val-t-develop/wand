#include "UnaryOperatorNode.hpp"


UnaryOperatorNode::UnaryOperatorNode(bool _isPre, string _op, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), isPre(_isPre), op(_op), expression(_expression) {}


shared_ptr<ClassRecord> UnaryOperatorNode::getReturnType() {
    return nullptr;
}
