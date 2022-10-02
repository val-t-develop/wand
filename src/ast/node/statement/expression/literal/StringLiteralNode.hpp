#pragma once
#include <Defs.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class StringLiteralNode : public ExpressionNode {
    string str;

    StringLiteralNode(string _str, shared_ptr<Node> _parent);
    shared_ptr<ClassRecord> getReturnType() ;
};

