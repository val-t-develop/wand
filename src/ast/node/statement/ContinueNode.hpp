#pragma once
#include <Defs.hpp>
#include <ast/node/statement/StatementNode.hpp>

class ContinueNode : public StatementNode {
public:
    ContinueNode(shared_ptr<Node> _parent);
};
