#include "AccessNode.hpp"

AccessNode::AccessNode(shared_ptr<Node> _parent) : ExpressionNode(_parent, NodeKind::ACCESS_NODE) {}

shared_ptr<ClassRecord> AccessNode::getReturnType() {
    if (isExpression()) {
        return static_pointer_cast<ExpressionNode>(access[access.size()-1])->getReturnType();
    } else {
        return nullptr;
    }
}