#include "TypeNode.hpp"


TypeNode::TypeNode(shared_ptr<AccessNode> _type, int _dims, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::TYPE_NODE), type(_type), dims(_dims) {}

shared_ptr<ClassRecord> TypeNode::getReturnType() {
    return nullptr;
}
