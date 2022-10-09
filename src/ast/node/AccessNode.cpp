#include "AccessNode.hpp"

AccessNode::AccessNode(shared_ptr<Node> _next, shared_ptr<Node> _child,
                       shared_ptr<Node> _parent)
    : AssignableNode(_parent, NodeKind::ACCESS_NODE), next(_next), child(_child) {}

shared_ptr<ClassRecord> AccessNode::getReturnType() {
    return nullptr;
}
