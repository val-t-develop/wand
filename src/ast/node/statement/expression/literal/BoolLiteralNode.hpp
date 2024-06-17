#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class BoolLiteralNode : public ExpressionNode {
  public:
    bool value;

    BoolLiteralNode(bool _value, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() override;
};
