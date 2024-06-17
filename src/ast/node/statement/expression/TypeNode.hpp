#pragma once
#include <Defs.hpp>
#include <ast/node/AccessNode.hpp>
#include <ast/node/ClassRecordNode.hpp>
#include <ast/node/statement/expression/ExpressionNode.hpp>

class TypeNode : public ExpressionNode {
  public:
    shared_ptr<ClassRecordNode> type;
    int dims;

    TypeNode(shared_ptr<ClassRecordNode> _type, int _dims,
             shared_ptr<Node> _parent);

    shared_ptr<ClassRecord> getReturnType() override;

    string getFullName();
};
