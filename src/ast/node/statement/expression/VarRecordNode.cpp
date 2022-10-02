#include "VarRecordNode.hpp"


VarRecordNode::VarRecordNode(shared_ptr<VarRecord> _record, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), record(_record) {}

shared_ptr<ClassRecord> VarRecordNode::getReturnType() {
    return nullptr;
}
