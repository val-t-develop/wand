#include "BlockNode.hpp"

BlockNode::BlockNode(vector<shared_ptr<Node>> _nodes, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::BLOCK_NODE), nodes(_nodes) {}
