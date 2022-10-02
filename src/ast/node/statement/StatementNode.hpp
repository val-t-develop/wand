#pragma once
#include <ast/node/Node.hpp>

class StatementNode : public Node {
public:
    StatementNode(shared_ptr<Node> _parent);
};
