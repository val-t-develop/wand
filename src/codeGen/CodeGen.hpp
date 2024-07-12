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
#include <IRTree/builder/IRTreeBuilder.hpp>
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
    Path file;
    shared_ptr<IRTree> tree;
    vector<DestructAfterStatement> destructAfterStatement = vector<DestructAfterStatement>();

    CodeGen(shared_ptr<IRTree> _tree, Path& _file);

    void codeGen();

    void build();
};
