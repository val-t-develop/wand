#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class MethodCallNode : public ExpressionNode {
public:
    shared_ptr<MethodRecord> record;
    vector<shared_ptr<ExpressionNode>> args = vector<shared_ptr<ExpressionNode>>();

    MethodCallNode(shared_ptr<MethodRecord> _record, vector<shared_ptr<ExpressionNode>> _args, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() ;
};
