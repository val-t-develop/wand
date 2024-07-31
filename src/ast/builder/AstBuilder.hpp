/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022-2024  Valentyn Tymchyshyn
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
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  Builder of Abstract Syntax Tree (AST).
 */

#pragma once
#include <ast/node/ClassDeclNode.hpp>
#include <ast/node/CompilationUnitNode.hpp>
#include <ast/node/ImportDeclNode.hpp>
#include <ast/node/PackageDeclNode.hpp>
#include <ast/node/VarsDeclNode.hpp>
#include <ast/node/statement/AssertNode.hpp>
#include <ast/node/statement/BreakNode.hpp>
#include <ast/node/statement/ContinueNode.hpp>
#include <ast/node/statement/ForEachNode.hpp>
#include <ast/node/statement/ForNode.hpp>
#include <ast/node/statement/IfElseNode.hpp>
#include <ast/node/statement/ReturnNode.hpp>
#include <ast/node/statement/WhileNode.hpp>
#include <ast/node/statement/expression/ArrayAccessNode.hpp>
#include <ast/node/statement/expression/ArrayCreationNode.hpp>
#include <ast/node/statement/expression/ArrayInitializerNode.hpp>
#include <ast/node/statement/expression/BinaryOperatorNode.hpp>
#include <ast/node/statement/expression/MethodCallNode.hpp>
#include <ast/node/statement/expression/NewNode.hpp>
#include <ast/node/statement/expression/UnaryOperatorNode.hpp>
#include <ast/node/statement/expression/VarRecordNode.hpp>
#include <lexer/Lexer.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <utils/Path.hpp>

class AstBuilder {
  public:
    shared_ptr<SymbolTable> symbolTable;
    stack<shared_ptr<ClassRecord>> classesStack = stack<shared_ptr<ClassRecord>>();
    Path filePath;
    Lexer lexer;

    AstBuilder(shared_ptr<SymbolTable> _symbolTable, Path _filePath,
               Lexer _lexer);

    shared_ptr<CompilationUnitNode> walk();
    shared_ptr<PackageDeclNode> enterPackage();
    shared_ptr<ImportDeclNode> enterImport();
    shared_ptr<ClassDeclNode> enterTypeDecl();
    shared_ptr<ClassDeclNode> enterClassDecl(ClassDeclNode::ClassKind kind);
    shared_ptr<ClassDeclNode> enterInterfaceDecl();
    shared_ptr<ClassDeclNode> enterEnumDecl();
    shared_ptr<GenericNode> enterGenericDecl();
    shared_ptr<ClassRecordNode> enterGenericTypeDecl();
    shared_ptr<Node> enterClassMemberDecl();
    shared_ptr<ConstructorDeclNode> enterConstructorDecl();
    shared_ptr<DestructorDeclNode> enterDestructorDecl();
    shared_ptr<MethodDeclNode> enterMethodDecl(shared_ptr<TypeNode> type,
                                               shared_ptr<MethodRecord> record, vector<ModifiersNode::ModifierKind> mods);
    shared_ptr<VarDeclNode> enterFieldDecl(shared_ptr<TypeNode> type,
                                           shared_ptr<VarRecord> record);
    vector<shared_ptr<VarDeclNode>> enterMethodArgs();
    shared_ptr<StatementNode> enterStatement();
    shared_ptr<StatementNode> enterNotVarStartement();
    shared_ptr<VarsDeclNode> enterLocalVarDecl(shared_ptr<ModifiersNode> mods);
    shared_ptr<BlockNode> enterBlockStatement(bool enterScope);
    shared_ptr<AssertNode> enterAssertStatement();
    shared_ptr<BreakNode> enterBreakStatement();
    shared_ptr<ContinueNode> enterContinueStatement();
    shared_ptr<ReturnNode> enterReturnStatement();
    shared_ptr<IfElseNode> enterIfStatement();
    shared_ptr<WhileNode> enterWhileStatement();
    shared_ptr<ForNode> enterForStatement();
    shared_ptr<ForEachNode> enterForEachStatement();
    shared_ptr<ExpressionNode> enterExpression();
    shared_ptr<ExpressionNode> enterBinOpRHS(int exprPrec,
                                             shared_ptr<ExpressionNode> LHS);
    shared_ptr<ExpressionNode> enterUnOpPrimary();
    shared_ptr<ExpressionNode> enterPrimary();
    shared_ptr<ExpressionNode> enterNew();
    shared_ptr<ExpressionNode> enterSuper();
    shared_ptr<ExpressionNode> enterParenExpression();
    shared_ptr<AccessNode> enterAccessOrCall(bool arr = false);
    void enterAccessWithoutArray(shared_ptr<AccessNode> access);
    void enterGeneric(shared_ptr<ClassRecordNode> classRecordNode);
    shared_ptr<ExpressionNode> enterLiteral();
    shared_ptr<ArrayInitializerNode> enterArrayInitializer();
    shared_ptr<TypeNode> enterType(bool arr);
    vector<shared_ptr<TypeNode>> enterTypeList();
    shared_ptr<ModifiersNode> enterModifiers();
    shared_ptr<MethodCallNode> enterMethodCall(shared_ptr<MethodRecord> methodRecord);

    static map<string, ModifiersNode::ModifierKind> modKinds;
};
