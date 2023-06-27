#pragma once
#include <Defs.hpp>

class Node {
public:
    enum class NodeKind {
        ACCESS_NODE,
        CLASS_DECL_NODE,
        CLASS_RECORD_NODE,
        COMPILATION_UNIT_NODE,
        GENERIC_NODE,
        IMPORT_DECL_NODE,
        METHOD_DECL_NODE,
        CONSTRUCTOR_DECL_NODE,
        MODIFIERS_NODE,
        PACKAGE_DECL_NODE,
        VAR_DECL_NODE,
        VARS_DECL_NODE,
        ASSERT_NODE,
        BLOCK_NODE,
        BREAK_NODE,
        CONTINUE_NODE,
        FOR_EACH_NODE,
        FOR_NODE,
        IF_ELSE_NODE,
        RETURN_NODE,
        WHILE_NODE,
        ARRAY_ACCESS_NODE,
        ARRAY_CREATION_NODE,
        ARRAY_INITIALIZER_NODE,
        BINARY_OPERATOR_NODE,
        CAST_NODE,
        METHOD_CALL_NODE,
        NEW_NODE,
        TYPE_NODE,
        UNARY_OPERATOR_NODE,
        VAR_RECORD_NODE,
        BOOL_LITERAL_NODE,
        CHAR_LITERAL_NODE,
        FLOAT_LITERAL_NODE,
        INT_LITERAL_NODE,
        NULL_LITERAL_NODE,
        STRING_LITERAL_NODE
    };

    NodeKind kind;

    shared_ptr<Node> parent;
    Node(shared_ptr<Node> _parent, NodeKind _kind);

    NodeKind getKind();
    bool isStatement();
    bool isExpression();
    bool isAssignable();
    bool isLiteral();
};
