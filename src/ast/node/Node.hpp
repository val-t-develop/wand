#pragma once
#include <Defs.hpp>

class Node {
public:
    shared_ptr<Node> parent;
    Node(shared_ptr<Node> _parent);
};
