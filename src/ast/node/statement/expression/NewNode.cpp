#include "NewNode.hpp"

NewNode::NewNode(shared_ptr<TypeNode> _type,
                 vector<shared_ptr<ExpressionNode>> _args, bool _isStatic,
                 shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::NEW_NODE), type(_type), args(_args),
      isStatic(_isStatic) {}

shared_ptr<ClassRecord> NewNode::getReturnType() { return nullptr; }
