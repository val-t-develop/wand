#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class FloatLiteralNode : public ExpressionNode {
  public:
    long double value;
    bool doubleVal;

    FloatLiteralNode(long double _value, bool _doubleVal,
                     shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;
};
