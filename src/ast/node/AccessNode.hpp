#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/AssignableNode.hpp>

class AccessNode : public AssignableNode {
public:
    shared_ptr<Node> next;
    shared_ptr<Node> child;

    AccessNode(shared_ptr<Node> _next, shared_ptr<Node> _child, shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;
};
