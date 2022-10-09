#include "PackageDeclNode.hpp"

PackageDeclNode::PackageDeclNode(vector<string> _name, shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::PACKAGE_DECL_NODE), name(_name) {}
