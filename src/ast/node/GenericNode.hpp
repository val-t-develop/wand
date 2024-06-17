#pragma once
#include <Defs.hpp>
#include <ast/node/ClassRecordNode.hpp>
#include <ast/node/Node.hpp>

class GenericNode : public Node {
  public:
    vector<shared_ptr<ClassRecordNode>> typeDecls =
        vector<shared_ptr<ClassRecordNode>>();

    GenericNode(vector<shared_ptr<ClassRecordNode>> _typeDecls,
                shared_ptr<Node> _parent);
};
