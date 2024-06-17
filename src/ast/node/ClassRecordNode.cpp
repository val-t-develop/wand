#include "ClassRecordNode.hpp"

ClassRecordNode::ClassRecordNode(shared_ptr<ClassRecord> _record,
                                 vector<shared_ptr<AccessNode>> _typeArgs,
                                 shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::CLASS_RECORD_NODE), record(_record),
      typeArgs(_typeArgs) {}

shared_ptr<ClassRecord> ClassRecordNode::getReturnType() { return record; }

string ClassRecordNode::getFullName() { return record->getFullName(); }
