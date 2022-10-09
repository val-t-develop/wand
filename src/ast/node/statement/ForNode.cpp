#include "ForNode.hpp"

ForNode::ForNode(shared_ptr<StatementNode> _init, shared_ptr<ExpressionNode> _condition,
                 shared_ptr<StatementNode> _update, shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::FOR_NODE), init(_init), condition(_condition), update(_update), statement(_statement) {}
