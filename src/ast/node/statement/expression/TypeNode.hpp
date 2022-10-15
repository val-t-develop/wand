#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/AccessNode.hpp>

class TypeNode : public ExpressionNode {
public:
    shared_ptr<AccessNode> type;
    int dims;

    TypeNode(shared_ptr<AccessNode> _type, int _dims, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;
};
