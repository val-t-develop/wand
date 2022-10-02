#include "GenericNode.hpp"

GenericNode::GenericNode(vector<shared_ptr<ClassRecordNode>> _typeDecls,
                         shared_ptr<Node> _parent)
    : Node(_parent), typeDecls(_typeDecls) {}
