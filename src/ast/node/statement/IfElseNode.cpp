#include "IfElseNode.hpp"

IfElseNode::IfElseNode(shared_ptr<ExpressionNode> _condition,
                       shared_ptr<StatementNode> _thenNode,
                       shared_ptr<StatementNode> _elseNode,
                       shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::IF_ELSE_NODE), condition(_condition),
      thenNode(_thenNode), elseNode(_elseNode) {}
