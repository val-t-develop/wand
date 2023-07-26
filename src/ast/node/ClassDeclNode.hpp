#pragma once
#include <Defs.hpp>
#include <ast/node/Node.hpp>
#include <ast/node/statement/expression/TypeNode.hpp>
#include <ast/node/VarDeclNode.hpp>
#include <ast/node/ModifiersNode.hpp>
#include <ast/node/GenericNode.hpp>
#include <ast/node/MethodDeclNode.hpp>
#include <ast/node/ConstructorDeclNode.hpp>
#include <symbolTable/SymbolTable.hpp>

class ClassDeclNode : public Node {
public:
    enum class ClassKind { CLASS, INTERFACE, ENUM };
    shared_ptr<GenericNode> generic;
    shared_ptr<ModifiersNode> modifiers;
    ClassKind kind;
    shared_ptr<ClassRecord> record;
    vector<shared_ptr<TypeNode>> extended = vector<shared_ptr<TypeNode>>();
    vector<shared_ptr<TypeNode>> implemented = vector<shared_ptr<TypeNode>>();
    vector<shared_ptr<VarDeclNode>> fields = vector<shared_ptr<VarDeclNode>>();
    vector<shared_ptr<MethodDeclNode>> methods =
                                        vector<shared_ptr<MethodDeclNode>>();
    vector<shared_ptr<ConstructorDeclNode>> constructors =
                                        vector<shared_ptr<ConstructorDeclNode>>();
    vector<shared_ptr<ClassDeclNode>> innerClasses =
                                       vector<shared_ptr<ClassDeclNode>>();

    ClassDeclNode(shared_ptr<GenericNode> _generic,
                  shared_ptr<ModifiersNode> _modifiers, ClassKind _kind,
                  shared_ptr<ClassRecord> _record,
                  vector<shared_ptr<TypeNode>> _extended,
                  vector<shared_ptr<TypeNode>> _implemented,
                  vector<shared_ptr<VarDeclNode>> _fields,
                  vector<shared_ptr<MethodDeclNode>> _methods,
                  vector<shared_ptr<ConstructorDeclNode>> _constructors,
                  vector<shared_ptr<ClassDeclNode>> _innerClasses,
                  shared_ptr<Node> _parent);

    string getFullName();
};
