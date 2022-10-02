#include "ImportDeclNode.hpp"

ImportDeclNode::ImportDeclNode(vector<string> _name, shared_ptr<Node> _parent)
    : Node(_parent), name(_name) {}
