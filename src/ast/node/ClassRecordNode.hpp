#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/AccessNode.hpp>

class ClassRecordNode : public ExpressionNode {
public:
    shared_ptr<ClassRecord> record;
    vector<shared_ptr<AccessNode>> typeArgs = vector<shared_ptr<AccessNode>>();

    ClassRecordNode(shared_ptr<ClassRecord> _record,
                    vector<shared_ptr<AccessNode>> _typeArgs,
                    shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() ;
};
