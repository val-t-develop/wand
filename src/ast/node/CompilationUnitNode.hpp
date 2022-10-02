#pragma once
#include <Defs.hpp>
#include <ast/node/Node.hpp>

class CompilationUnutNode : public Node {
public:
    vector<shared_ptr<Node>> nodes = vector<shared_ptr<Node>>();

    CompilationUnutNode();
};
