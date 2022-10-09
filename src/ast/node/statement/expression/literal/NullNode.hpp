#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class NullLiteralNode : public ExpressionNode {
public:
    NullLiteralNode(shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() ;
};
