#pragma once
#include <Defs.hpp>
#include <ast/node/Node.hpp>

class ModifiersNode : public Node {
public:
    enum class ModifierKind {
        PUBLIC, PRIVATE, PROTECTED, STATIC, FINAL, ABSTRACT, NATIVE, SYNCHRONIZED
    };

    vector<ModifierKind> modifiers = vector<ModifierKind>();

    ModifiersNode(shared_ptr<Node> _parent);
};
