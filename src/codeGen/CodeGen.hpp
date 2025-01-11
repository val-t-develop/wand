/*  WAND - Wand Programming Language compiler
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Parser of AST to llvm ir.
 */

#pragma once
#include <IRTree/builder/IRTreeBuilder.hpp>
#include <IRTree/node/statement/IRIfElse.hpp>
#include <IRTree/node/statement/IRLoop.hpp>
#include <IRTree/node/statement/expression/IRVar.hpp>
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

      DestructAfterStatement(Value *_val, string _type);
    };


    shared_ptr<CodeGenUtils> utils;
    shared_ptr<LLVMHelper> helper;
    Path file;
    shared_ptr<IRTree> tree;
    vector<DestructAfterStatement> destructAfterStatement = vector<DestructAfterStatement>();
    BasicBlock *retBB;
    shared_ptr<IRFunction> currFunction;
    map<string, string> varTypes;
    map<string, Value *> NamedValues;
    map<string, Value *> GlobalNamedValues;
    stack<vector<pair<Value *, string>>> currBlockVars =
        stack<vector<pair<Value *, string>>>();
    // TODO NamedValues and currBlockVars should be united
    bool isRef = false;

    CodeGen(shared_ptr<IRTree> _tree, Path& _file);

    void codeGen();

    void build();

    void genStructType(shared_ptr<IRStruct> node);
    void genStruct(shared_ptr<IRStruct> node);
    void genGlobalVar(shared_ptr<IRVarDecl> node);
    void genFunctionPrototype(shared_ptr<IRFunction> node);
    void genFunction(shared_ptr<IRFunction> node);
    bool genBlock(shared_ptr<IRBlock> node);
    bool genStatement(shared_ptr<IRStatement> node, bool makeRet);
    void genIfElse(shared_ptr<IRIfElse> node);
    void genLoop(shared_ptr<IRLoop> node);
    void genVarDecl(shared_ptr<IRVarDecl> node);
    Value *genExpression(shared_ptr<IRExpression> node, bool genRef);
    Value *genLiteral(shared_ptr<IRLiteral> node);
    Value *genCall(shared_ptr<IRCall> node);
    Value *genVarValue(shared_ptr<IRVar> node, bool genRef);
    Value *genBinOp(shared_ptr<IRBinOp> node);
};
