#include "VarDeclNode.hpp"

VarDeclNode::VarDeclNode(shared_ptr<ModifiersNode> _modifiers, shared_ptr<TypeNode> _type, shared_ptr<VarRecord> _record,
                         shared_ptr<ExpressionNode> _init, shared_ptr<Node> _parent)
    : StatementNode(_parent, NodeKind::VAR_DECL_NODE), modifiers(_modifiers), type(_type), record(_record), init(_init) {}
