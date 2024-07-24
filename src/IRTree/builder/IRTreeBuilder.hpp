/*  SPL - Simple Programming Language compiler
*  Copyright (C) 2022  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  Builder of ir tree.
 */

#pragma once
#include <Defs.hpp>
#include <IRTree/node/IRTree.hpp>
#include <IRTree/node/statement/expression/IRBinOp.hpp>
#include <IRTree/node/statement/expression/IRCall.hpp>
#include <IRTree/node/statement/expression/IRExpression.hpp>
#include <IRTree/node/statement/expression/IRLiteral.hpp>
#include <src/ast/builder/AstBuilder.hpp>

class IRTreeBuilder {
public:
    shared_ptr<CompilationUnitNode> cu;
    shared_ptr<IRTree> tree;
    Path file;

    vector<string> imported = vector<string>();
    stack<shared_ptr<ClassDeclNode>> classesStack = stack<shared_ptr<ClassDeclNode>>();
    vector<string> NamedValues = vector<string>();
    vector<string> GlobalNamedValues = vector<string>();

    IRTreeBuilder(shared_ptr<CompilationUnitNode> _cu, Path& _file);
    void walk();

    void enterImports(Path f);
    void enterClassDecl(shared_ptr<ClassDeclNode> node, bool genMethods);
    void enterMethodPrototype(shared_ptr<MethodDeclNode> node);
    void enterMethod(shared_ptr<MethodDeclNode> node);
    void enterConstructor(shared_ptr<ConstructorDeclNode> node, bool defaultConstrucctor);
    void enterConstructorPrototype(shared_ptr<ConstructorDeclNode> node);
    void enterDestructor(shared_ptr<ClassDeclNode> node);
    void enterDestructorPrototype(shared_ptr<ClassDeclNode> node);

    shared_ptr<IRBlock> enterBlock(shared_ptr<BlockNode> node);
    shared_ptr<IRStatement> enterStatement(shared_ptr<StatementNode> el);
    shared_ptr<IRExpression> enterExpression(shared_ptr<ExpressionNode> node);
    shared_ptr<IRLiteral> enterLiteral(shared_ptr<ExpressionNode> node);
    shared_ptr<IRCall> enterCall(shared_ptr<MethodCallNode> node);
    shared_ptr<IRBinOp> enterBinOp(shared_ptr<BinaryOperatorNode> node);
    shared_ptr<IRCall> enterNew(shared_ptr<NewNode> node);

    static shared_ptr<IRExpression> getDefaultValue(string type);
};
