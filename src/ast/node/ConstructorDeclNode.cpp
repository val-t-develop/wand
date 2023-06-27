#include "ConstructorDeclNode.hpp"

ConstructorDeclNode::ConstructorDeclNode(shared_ptr<ModifiersNode> _modifiers, shared_ptr<MethodRecord> _record,
                               vector<shared_ptr<VarDeclNode>> _args, shared_ptr<BlockNode> _body, shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::CONSTRUCTOR_DECL_NODE), modifiers(_modifiers), record(_record), args(_args), body(_body) {
    // TODO
}
