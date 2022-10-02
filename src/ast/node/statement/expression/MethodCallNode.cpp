#include "MethodCallNode.hpp"

MethodCallNode::MethodCallNode(shared_ptr<MethodRecord> _record, vector<shared_ptr<ExpressionNode>> _args, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), record(_record), args(_args) {}


shared_ptr<ClassRecord> MethodCallNode::getReturnType() {
    return nullptr;
}
