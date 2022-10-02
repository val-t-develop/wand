#pragma once
#include <Defs.hpp>
#include <ast/node/statement/StatementNode.hpp>

class BlockNode : public StatementNode {
public:
    vector<shared_ptr<Node>> nodes = vector<shared_ptr<Node>>();

    BlockNode(vector<shared_ptr<Node>> _nodes, shared_ptr<Node> _parent);
};
