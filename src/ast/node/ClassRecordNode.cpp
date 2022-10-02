#include "ClassRecordNode.hpp"

ClassRecordNode::ClassRecordNode(shared_ptr<ClassRecord> _record,
                                 vector<shared_ptr<AccessNode>> _typeArgs,
                                 shared_ptr<Node> _parent)
    : ExpressionNode(_parent), record(_record), typeArgs(_typeArgs) {}

shared_ptr<ClassRecord> ClassRecordNode::getReturnType() {
    return record;
}
