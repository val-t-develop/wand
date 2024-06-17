#include "ClassDeclNode.hpp"

ClassDeclNode::ClassDeclNode(
    shared_ptr<GenericNode> _generic, shared_ptr<ModifiersNode> _modifiers,
    ClassKind _kind, shared_ptr<ClassRecord> _record,
    vector<shared_ptr<TypeNode>> _extended,
    vector<shared_ptr<TypeNode>> _implemented,
    vector<shared_ptr<VarDeclNode>> _fields,
    vector<shared_ptr<MethodDeclNode>> _methods,
    vector<shared_ptr<ConstructorDeclNode>> _constructors,
    vector<shared_ptr<ClassDeclNode>> _innerClasses, shared_ptr<Node> _parent)
    : Node(_parent, NodeKind::CLASS_DECL_NODE), generic(_generic),
      modifiers(_modifiers), kind(_kind), record(_record), extended(_extended),
      implemented(_implemented), fields(_fields), methods(_methods),
      constructors(_constructors), innerClasses(_innerClasses) {}

string ClassDeclNode::getFullName() { return record->getFullName(); }
