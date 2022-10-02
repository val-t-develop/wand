#include "ExpressionNode.hpp"

ExpressionNode::ExpressionNode(shared_ptr<Node> _parent) : StatementNode(_parent) {}
shared_ptr<ClassRecord> getReturnType() {
    return nullptr;
}
ExpressionNode::~ExpressionNode() {}

