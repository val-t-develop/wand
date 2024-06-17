#include "Node.hpp"
#include <ast/node/AccessNode.hpp>

Node::Node(shared_ptr<Node> _parent, NodeKind _kind)
    : kind(_kind), parent(_parent) {}

Node::NodeKind Node::getKind() { return kind; }
bool Node::isStatement() {
    if (kind == NodeKind::CLASS_RECORD_NODE ||
        kind == NodeKind::VAR_DECL_NODE || kind == NodeKind::VARS_DECL_NODE ||
        kind == NodeKind::ASSERT_NODE || kind == NodeKind::BLOCK_NODE ||
        kind == NodeKind::BREAK_NODE || kind == NodeKind::CONTINUE_NODE ||
        kind == NodeKind::FOR_EACH_NODE || kind == NodeKind::FOR_NODE ||
        kind == NodeKind::IF_ELSE_NODE || kind == NodeKind::RETURN_NODE ||
        kind == NodeKind::WHILE_NODE || kind == NodeKind::ARRAY_ACCESS_NODE ||
        kind == NodeKind::ARRAY_INITIALIZER_NODE ||
        kind == NodeKind::ARRAY_CREATION_NODE ||
        kind == NodeKind::BINARY_OPERATOR_NODE || kind == NodeKind::CAST_NODE ||
        kind == NodeKind::METHOD_CALL_NODE || kind == NodeKind::NEW_NODE ||
        kind == NodeKind::UNARY_OPERATOR_NODE ||
        kind == NodeKind::VAR_RECORD_NODE ||
        kind == NodeKind::BOOL_LITERAL_NODE ||
        kind == NodeKind::CHAR_LITERAL_NODE ||
        kind == NodeKind::FLOAT_LITERAL_NODE ||
        kind == NodeKind::INT_LITERAL_NODE ||
        kind == NodeKind::NULL_LITERAL_NODE ||
        kind == NodeKind::STRING_LITERAL_NODE) {
        return true;
    } else if (kind == NodeKind::ACCESS_NODE) {
        AccessNode *an = static_cast<AccessNode *>(this);
        if (an->access.size() == 1) {
            if (an->access[0]->getKind() == NodeKind::CLASS_RECORD_NODE) {
                return false;
            }
        }
        return true;
    }
    return false;
}
bool Node::isExpression() {
    if (kind == NodeKind::ARRAY_ACCESS_NODE ||
        kind == NodeKind::CLASS_RECORD_NODE ||
        kind == NodeKind::ARRAY_INITIALIZER_NODE ||
        kind == NodeKind::ARRAY_CREATION_NODE ||
        kind == NodeKind::BINARY_OPERATOR_NODE || kind == NodeKind::CAST_NODE ||
        kind == NodeKind::METHOD_CALL_NODE || kind == NodeKind::NEW_NODE ||
        kind == NodeKind::UNARY_OPERATOR_NODE ||
        kind == NodeKind::VAR_RECORD_NODE ||
        kind == NodeKind::BOOL_LITERAL_NODE ||
        kind == NodeKind::CHAR_LITERAL_NODE ||
        kind == NodeKind::FLOAT_LITERAL_NODE ||
        kind == NodeKind::INT_LITERAL_NODE ||
        kind == NodeKind::NULL_LITERAL_NODE ||
        kind == NodeKind::STRING_LITERAL_NODE) {
        return true;
    } else if (kind == NodeKind::ACCESS_NODE) {
        AccessNode *an = static_cast<AccessNode *>(this);
        if (an->access.size() == 1) {
            if (an->access[0]->getKind() == NodeKind::CLASS_RECORD_NODE) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Node::isLiteral() {
    if (kind == NodeKind::BOOL_LITERAL_NODE ||
        kind == NodeKind::CHAR_LITERAL_NODE ||
        kind == NodeKind::FLOAT_LITERAL_NODE ||
        kind == NodeKind::INT_LITERAL_NODE ||
        kind == NodeKind::NULL_LITERAL_NODE ||
        kind == NodeKind::STRING_LITERAL_NODE) {
        return true;
    }
    return false;
}
