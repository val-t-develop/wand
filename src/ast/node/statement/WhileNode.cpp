#include "WhileNode.hpp"

WhileNode::WhileNode(shared_ptr<ExpressionNode> _expression, shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::WHILE_NODE), expression(_expression), statement(_statement) {}
