#include "ModifiersNode.hpp"

ModifiersNode::ModifiersNode(shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::MODIFIERS_NODE) {}
