#pragma once
#include <Defs.hpp>
#include <ast/node/Node.hpp>

class PackageDeclNode : public Node {
public:
    vector<string> name = vector<string>();

    PackageDeclNode(vector<string> _name, shared_ptr<Node> _parent);
};
