#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>

class CastExpression : public ExpressionNode {
public:
    shared_ptr<TypeNode> type;
    shared_ptr<ExpressionNode> expression;

    CastExpression(shared_ptr<TypeNode> _type, shared_ptr<ExpressionNode> _expression, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() ;
};
