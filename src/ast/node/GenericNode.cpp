#include "GenericNode.hpp"

GenericNode::GenericNode(vector<shared_ptr<ClassRecordNode>> _typeDecls,
                         shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::GENERIC_NODE), typeDecls(_typeDecls) {}
