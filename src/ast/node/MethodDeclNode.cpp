#include "MethodDeclNode.hpp"

MethodDeclNode::MethodDeclNode(shared_ptr<ModifiersNode> _modifiers,
                               shared_ptr<TypeNode> _returnType,
                               shared_ptr<MethodRecord> _record,
                               vector<shared_ptr<VarDeclNode>> _args,
                               shared_ptr<BlockNode> _body,
                               shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::METHOD_DECL_NODE), modifiers(_modifiers),
      returnType(_returnType), record(_record), args(_args), body(_body) {
    // TODO
}

string MethodDeclNode::getFullName() {
    if (record->ir_name == "") {
        string str = "";
        if (record->next != nullptr) {
            str +=
                static_pointer_cast<ClassRecord>(record->next)->getFullName() +
                ".";
        }
        str += record->id;

        str += "__spl__" + returnType->type->record->getFullName();
        for (auto arg : args) {
            str += "__" + arg->type->type->record->getFullName();
        }
        record->ir_name = str;
    }
    return record->ir_name;
}
