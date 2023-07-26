#include "TypeNode.hpp"


TypeNode::TypeNode(shared_ptr<ClassRecordNode> _type, int _dims, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::TYPE_NODE), type(_type), dims(_dims) {}

shared_ptr<ClassRecord> TypeNode::getReturnType() {
    return nullptr;
}

string TypeNode::getFullName() {
    return type->getFullName();
}
