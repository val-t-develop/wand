#pragma once
#include <Defs.hpp>
#include <ast/node/statement/StatementNode.hpp>

class BreakNode : public StatementNode {
public:
    BreakNode(shared_ptr<Node> _parent);
};
