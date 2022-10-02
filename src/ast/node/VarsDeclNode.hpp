#pragma once
#include <Defs.hpp>
#include <ast/node/Node.hpp>


class VarsDeclNode : public Node {
public:
    vector<shared_ptr<VarsDeclNode>> decls;

    VarsDeclNode(vector<shared_ptr<VarsDeclNode>> _decls, shared_ptr<Node> _parent);
};
