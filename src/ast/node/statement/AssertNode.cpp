#include "AssertNode.hpp"

AssertNode::AssertNode(shared_ptr<ExpressionNode> _condition, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::ASSERT_NODE), condition(_condition), expression(_expression) {}
