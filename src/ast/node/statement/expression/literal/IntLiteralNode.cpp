#include "IntLiteralNode.hpp"

IntLiteralNode::IntLiteralNode(int64_t _value, bool _longVal,
                               shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::INT_LITERAL_NODE), value(_value),
      longVal(_longVal) {}

shared_ptr<ClassRecord> IntLiteralNode::getReturnType() { return nullptr; }
