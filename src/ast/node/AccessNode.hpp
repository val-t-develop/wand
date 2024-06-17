#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/AssignableNode.hpp>

class AccessNode : public ExpressionNode {
  public:
    vector<shared_ptr<Node>> access = vector<shared_ptr<Node>>();

    AccessNode(shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;
};
