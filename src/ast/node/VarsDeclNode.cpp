#include "VarsDeclNode.hpp"

VarsDeclNode::VarsDeclNode(vector<shared_ptr<VarDeclNode>> _decls,
                           shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::VARS_DECL_NODE), decls(_decls) {}
