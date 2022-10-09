#pragma once
#include <ast/node/VarDeclNode.hpp>


class VarsDeclNode : public StatementNode {
public:
    vector<shared_ptr<VarDeclNode>> decls = vector<shared_ptr<VarDeclNode>>();

    VarsDeclNode(vector<shared_ptr<VarDeclNode>> _decls, shared_ptr<Node> _parent);
};
