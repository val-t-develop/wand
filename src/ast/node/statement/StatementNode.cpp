#include "StatementNode.hpp"

StatementNode::StatementNode(shared_ptr<Node> _parent, NodeKind _kind)
    : Node(_parent, _kind) {}
