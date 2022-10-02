#include "VarsDeclNode.hpp"

VarsDeclNode::VarsDeclNode(vector<shared_ptr<VarsDeclNode>> _decls, shared_ptr<Node> _parent) : decls(_decls), Node(_parent) {}
