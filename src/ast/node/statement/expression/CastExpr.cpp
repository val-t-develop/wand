#include "CastExpr.hpp"


CastExpression::CastExpression(shared_ptr<TypeNode> _type, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent)
    : ExpressionNode(_parent), type(_type), expression(_expression) {}

shared_ptr<ClassRecord> CastExpression::getReturnType() {
    return nullptr;
}
