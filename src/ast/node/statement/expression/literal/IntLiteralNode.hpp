#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class IntLiteralNode : public ExpressionNode {
public:
    int64_t value;
    bool longVal;

    IntLiteralNode(int64_t _value, bool _longVal, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};
