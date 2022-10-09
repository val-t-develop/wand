#include "ArrayCreationNode.hpp"

ArrayCreationNode::ArrayCreationNode(shared_ptr<TypeNode> _type, vector<shared_ptr<ExpressionNode>> _dims,
                                     shared_ptr<ArrayInitializerNode> _init, shared_ptr<Node> _parent)
    : ExpressionNode(_parent, NodeKind::ARRAY_CREATION_NODE), type(_type), dims(_dims), init(_init) {}


shared_ptr<ClassRecord> ArrayCreationNode::getReturnType() {
    return nullptr;
}
