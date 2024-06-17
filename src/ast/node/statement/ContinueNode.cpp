#include "ContinueNode.hpp"

ContinueNode::ContinueNode(shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::CONTINUE_NODE) {}
