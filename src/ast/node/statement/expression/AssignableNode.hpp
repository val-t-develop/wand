#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class AssignableNode : public ExpressionNode {
  public:
    AssignableNode(shared_ptr<Node> _parent, NodeKind _kind);
    shared_ptr<ClassRecord> getReturnType() override;
};
