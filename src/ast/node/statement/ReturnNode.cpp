#include "ReturnNode.hpp"

ReturnNode::ReturnNode(shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::RETURN_NODE), expression(_expression) {}
