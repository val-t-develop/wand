#include "BreakNode.hpp"

BreakNode::BreakNode(shared_ptr<Node> _parent) : StatementNode(_parent, NodeKind::BREAK_NODE) {}
