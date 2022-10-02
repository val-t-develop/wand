#include "MethodDeclNode.hpp"

MethodDeclNode::MethodDeclNode(shared_ptr<ModifiersNode> _modifiers, shared_ptr<TypeNode> _returnType, shared_ptr<MethodRecord> _record,
                               vector<shared_ptr<VarDeclNode>> _args, shared_ptr<BlockNode> _body, shared_ptr<Node> _parent)
    : Node(_parent), modifiers(_modifiers), returnType(_returnType), record(_record), args(_args), body(_body) {
    // TODO
}
