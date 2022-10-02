#include "NewNode.hpp"


NewNode::NewNode(shared_ptr<AccessNode> _type, vector<shared_ptr<ExpressionNode>> _args, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), type(_type), args(_args) {}

shared_ptr<ClassRecord> NewNode::getReturnType() {
    return nullptr;
}
