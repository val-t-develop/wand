#include "ForEachNode.hpp"

ForEachNode::ForEachNode(shared_ptr<VarDeclNode> _var, shared_ptr<ExpressionNode> _expression,
                         shared_ptr<StatementNode> _statement, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::FOR_EACH_NODE), var(_var), expression(_expression), statement(_statement) {}
