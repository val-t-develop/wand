#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/AccessNode.hpp>

class NewNode : public ExpressionNode {
public:
    shared_ptr<AccessNode> type;
    vector<shared_ptr<ExpressionNode>> args = vector<shared_ptr<ExpressionNode>>();

    NewNode(shared_ptr<AccessNode> _type, vector<shared_ptr<ExpressionNode>> _args, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() ;
};
