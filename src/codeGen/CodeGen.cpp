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

#include "CodeGen.hpp"
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <main.hpp>
#include <utils/Out.hpp>

#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/PassBuilder.h>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Host.h"

#include <ast/node/statement/expression/literal/StringLiteralNode.hpp>

CodeGen::CodeGen(shared_ptr<IRTree> _tree, Path& _file) : tree(_tree), file(_file) {
    string moduleName = "__unnamedModule";
    helper = make_shared<LLVMHelper>(moduleName);
    utils = make_shared<CodeGenUtils>(this);
}

void CodeGen::codeGen() {

}

void CodeGen::build() {

}