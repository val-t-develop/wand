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
 *  Parser of AST to llvm ir.
 */

#pragma once
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/utils/CodeGenUtils.hpp>
#include <codeGen/utils/LLVMHelper.hpp>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

using namespace llvm;

class CodeGenUtils;

class CodeGen {
  public:
    class DestructAfterStatement {
    public:
        Value *val;
        string type;
        bool decreaseRefs;

        DestructAfterStatement(Value *_val, string _type, bool _decreaseRefs);
    };
    shared_ptr<CodeGenUtils> utils;
    shared_ptr<LLVMHelper> helper;

    vector<string> imported = vector<string>();
    map<string, Value *> NamedValues;
    map<string, Value *> GlobalNamedValues;
    map<Value *, shared_ptr<ExpressionNode>> StaticGlobalsInit;
    map<shared_ptr<VarRecord>, Type *> varTypes;
    BasicBlock *retBB{};
    stack<Value *> thisV;
    shared_ptr<VarRecord> thisRecord;
    vector<DestructAfterStatement> destructAfterStatement = vector<DestructAfterStatement>();

    shared_ptr<CompilationUnitNode> cu;

    stack<shared_ptr<ClassDeclNode>> classesStack =
        stack<shared_ptr<ClassDeclNode>>();
    stack<vector<pair<Value *, string>>> currBlockVars =
        stack<vector<pair<Value *, string>>>();
    Path file;
    shared_ptr<MethodDeclNode> currMethod;
    bool isRef = false;

    CodeGen(shared_ptr<CompilationUnitNode> _cu, Path& _file);

    void codeGen();

    void build();
    void genImport(Path f);
    void createClassType(shared_ptr<ClassDeclNode> node);
    void genClassDecl(shared_ptr<ClassDeclNode> node, bool genMethod);

    void genStruct(shared_ptr<ClassDeclNode> node);

    Function *genMethodPrototype(shared_ptr<MethodDeclNode> node);
    Function *genDestructorPrototype(shared_ptr<ClassDeclNode> node);
    Function *genConstructorPrototype(shared_ptr<ConstructorDeclNode> node);
    Function *genMethodDecl(shared_ptr<MethodDeclNode> node);
    Function *genDestructorDecl(shared_ptr<ClassDeclNode> node);
    Function *genConstructorDecl(shared_ptr<ConstructorDeclNode> node);
    bool genBlockStatement(shared_ptr<BlockNode> node);
    bool
    genConstructorBlockStatement(shared_ptr<ConstructorDeclNode> constructor);
    void genIfElse(shared_ptr<IfElseNode> node);
    void genWhile(shared_ptr<WhileNode> node);
    void genFor(shared_ptr<ForNode> node);
    Value *genExpression(shared_ptr<ExpressionNode> node, bool genRef);
    Value *genLiteral(shared_ptr<ExpressionNode> node);
    Value *genMethodCall(shared_ptr<MethodCallNode> node, Value *calle);
    Value *genVarDecl(shared_ptr<VarDeclNode> node);
    Value *genDefaultValue(shared_ptr<TypeNode> node);
    Value *genVarValue(shared_ptr<VarRecordNode> node, bool genRef);
    Value *genBinOp(shared_ptr<BinaryOperatorNode> node);
    Value *genNewNode(shared_ptr<NewNode> node);
};
