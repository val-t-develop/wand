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

#include <IRTree/node/statement/IRIfElse.hpp>
#include <IRTree/node/statement/IRReturn.hpp>
#include <IRTree/node/statement/IRWhile.hpp>
#include <ast/node/statement/expression/literal/StringLiteralNode.hpp>

CodeGen::CodeGen(shared_ptr<IRTree> _tree, Path& _file) : tree(_tree), file(_file) {
    if (tree->moduleName=="spl.core") {
        auto dir = file.getParent();
        auto ll_file = Path(dir.getName() + "/spl.core.stdlib.ll");
        system(string("mkdir -p .spl_compilation"+dir.getName()).c_str());
        string o_file = ".spl_compilation" + dir.getName() + "/spl.core.stdlib.o";
        Main::currCUsStack.top()->linkingObj.push_back(o_file);
        system(string("clang " + ll_file.getName() + " -c -o " + o_file).c_str());
    }
    helper = make_shared<LLVMHelper>(tree->moduleName);
    utils = make_shared<CodeGenUtils>(this);
}

void CodeGen::codeGen() {
    helper->createFunctionPrototype("__spl__init__gc", helper->getVoidType(),
                                    vector<Type *>{});
    helper->createFunctionPrototype("__spl__destroy__gc",
                                    helper->getVoidType(), vector<Type *>{});
    helper->createFunctionPrototype(
        "__spl__alloc", helper->getPointerType(helper->getVoidType()),
        vector<Type *>{helper->getIntType(32)});
    helper->createFunctionPrototype(
        "__spl__destroyobj", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
            helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype(
        "__spl__destroyref", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
            helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype(
        "__spl__addref", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
                       helper->getPointerType(helper->getVoidType()),
                       helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype(
        "__spl__write", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
                       helper->getPointerType(helper->getVoidType()),
                       helper->getPointerType(helper->getVoidType())});
    for (auto el : tree->structs) {
        genStructType(el);
    }
    for (auto el : tree->structs) {
        genStruct(el);
    }
    for (auto el : tree->globalVars) {
        genGlobalVar(el);
    }
    for (auto el : tree->funcs) {
        genFunctionPrototype(el);
    }
    for (auto el : tree->funcs) {
        genFunction(el);
    }
}

void CodeGen::build() {
    helper->printModule();
    helper->prepareBuild();
    helper->runPasses();
    system(string("mkdir -p .spl_compilation"+file.getParent().getName()).c_str());
    string Filename = ".spl_compilation"+file.getName()+".o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
    }

    helper->build(dest);
    Main::obj_files.push_back(Filename);
}

void CodeGen::genStructType(shared_ptr<IRStruct> node) {
    StructType *structType = helper->createStructType(node->name);
    utils->classesTypes.insert({node->name, structType});
}

void CodeGen::genStruct(shared_ptr<IRStruct> node) {
    vector<Type *> types = vector<Type *>();

    for (auto var : node->fields) {
        types.push_back(utils->getType(var->type));
        varTypes[var->name] = utils->getType(var->type);
    }

    string fullName = node->name;

    StructType *structType;
    if (utils->classesTypes.contains(fullName)) {
        structType = utils->classesTypes.at(fullName);
    } else {
        structType = nullptr;
        Out::errorMessage("Can not get " + fullName);
    }
    structType->setBody(types);
}

void CodeGen::genGlobalVar(shared_ptr<IRVarDecl> node) {
    GlobalVariable *ptr = helper->createGlobalVar(
        helper->getPointerType(utils->getType(node->type)), node->name);
    GlobalNamedValues[node->name] = ptr;
    ConstantPointerNull *const_ptr_2 =
        helper->getNullptr(helper->getPointerType(utils->getType(node->type)));
    ptr->setInitializer(const_ptr_2);
    // TODO StaticGlobalsInit[ptr] = node->init;
}

void CodeGen::genFunctionPrototype(shared_ptr<IRFunction> node) {
    vector<Type *> args_types = vector<Type *>();

    for (int i = 0; i < node->args.size(); ++i) {
        auto arg = node->args[i];
        args_types.push_back(utils->getType(arg->type));
    }
    helper->createFunctionPrototype(node->name, utils->getType(node->type), args_types);
}

void CodeGen::genFunction(shared_ptr<IRFunction> node) {
    currFunction=node;
    if (node->body != nullptr) {
        Function *TheFunction = helper->getFunction(node->name);

        vector<Type *> args_types = vector<Type *>();

        for (int i = 0; i < node->args.size(); ++i) {
            auto arg = node->args[i];
            args_types.push_back(utils->getType(arg->type));
        }

        if (!TheFunction) {
            return;
        }

        BasicBlock *BB = helper->createBBinFunc("entry", TheFunction);
        retBB = helper->createBBinFunc("ret", TheFunction);
        helper->activateBB(BB);

        NamedValues.clear();

        Value *ret_ptr;
        if (TheFunction->getReturnType() != helper->getVoidType()) {
            ret_ptr = helper->createAlloca(TheFunction->getReturnType(),
                                           nullptr, "retallocatmp");
            NamedValues[node->name + "__spl__ret"] = ret_ptr;
            helper->createStore(
                genExpression(IRTreeBuilder::getDefaultValue(node->type), false),
                ret_ptr);
        }
        for (int i = 0; i < node->args.size(); ++i) {
            auto Arg = TheFunction->getArg(i);
            string argName = node->args[i]->name;
            Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
            helper->createStore(Arg, ptr);
            NamedValues[argName] = ptr;
            varTypes[node->args[i]->name] = Arg->getType();
        }

        bool br = genBlock(node->body);
        if (!br) {
            helper->createBr(retBB);
        }

        helper->activateBB(retBB);

        if (TheFunction->getReturnType() != helper->getVoidType()) {
            Value *ret_val = helper->createLoad(TheFunction->getReturnType(),
                                                ret_ptr, "retloadtmp");
            helper->createRet(ret_val);
        } else {
            helper->createRet(UndefValue::get(helper->getVoidType()));
        }

        verifyFunction(*TheFunction);

        if (node->type == "int") {
            if (node->name == "main") {
                if (node->args.size() == 1) {
                    if (node->args[0]->type == "String") {
                        Function *MainFunction =
                            helper->createFunctionPrototype(
                                "main", helper->getIntType(32),
                                vector<Type *>());

                        if (!MainFunction) {
                            return;
                        }

                        BasicBlock *mainBB =
                            helper->createBBinFunc("entry", MainFunction);
                        helper->activateBB(mainBB);

                        helper->createCall("__spl__init__gc");
                        // TODO
                        /*for (auto globInit : StaticGlobalsInit) {
                            if (globInit.second != nullptr) {
                                Value *ptr = globInit.first;
                                Value *val = genExpression(globInit.second,
                        false); helper->createStore(val, ptr);
                            }
                        }*/

                        Value *RetVal = helper->createCall(
                            TheFunction,
                            vector<Value *>({helper->getNullptr(
                                static_cast<PointerType *>(args_types[0]))}));

                        helper->createCall("__spl__destroy__gc");

                        if (RetVal) {
                            helper->createRet(RetVal);
                            verifyFunction(*MainFunction);
                        }
                    }
                }
            }
        }
    }
}

bool CodeGen::genBlock(shared_ptr<IRBlock> node) {
    bool ret = false;
    currBlockVars.push(vector<pair<Value *, string>>());
    for (auto item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == IRNode::Kind::BLOCK) {
                ret = genBlock(static_pointer_cast<IRBlock>(item));
                utils->destructAfterStatement();
                break;
            } else if (item->kind == IRNode::Kind::RETURN) {
                if (static_pointer_cast<IRReturn>(item)->val != nullptr) {
                    Value *ptr =
                        NamedValues[helper->getCurrFunction()->getName().str() +
                                    "__spl__ret"];
                    Value *val = genExpression(
                        static_pointer_cast<IRReturn>(item)->val, false);
                    if (val->getType()->isPointerTy()) {
                        helper->createCall(
                            "__spl__addref",
                            vector<Value *>{
                                ptr, val,
                                helper->getFunction("__spl__destructor__" +
                                                    currFunction->type)});
                    }
                    helper->createStore(val, ptr);
                }
                ret = true;
                utils->destructAfterStatement();
                break;
            } else if (item->kind == IRNode::Kind::VAR_DECL) {
                genVarDecl(static_pointer_cast<IRVarDecl>(item));
                utils->destructAfterStatement();
            } else if (item->isExpression()) {
                genExpression(static_pointer_cast<IRExpression>(item), false);
                utils->destructAfterStatement();
            } else if (item->kind == IRNode::Kind::IF_ELSE) {
                genIfElse(static_pointer_cast<IRIfElse>(item));
                utils->destructAfterStatement();
            } else if (item->kind == IRNode::Kind::WHILE) {
                genWhile(static_pointer_cast<IRWhile>(item));
                utils->destructAfterStatement();
            }
        }
    }

    for (auto v : currBlockVars.top()) {
        if (v.second != "bool" && v.second != "int" && v.second != "byte" &&
            v.second != "short" && v.second != "long" && v.second != "float" &&
            v.second != "double" && v.second != "char" && v.second != "void") {

            helper->createCall(
                "__spl__destroyref",
                vector<Value *>{
                    v.first,
                    helper->getFunction("__spl__destructor__" + v.second)});
        }
    }
    if (ret) {
        helper->createBr(retBB);
    }
    currBlockVars.pop();
    return ret;
}

void CodeGen::genIfElse(shared_ptr<IRIfElse> node) {
    Value *cond = genExpression(node->cond, false);

    Function *TheFunction = helper->getCurrFunction();

    // Create blocks for the then and else cases.  Insert the 'then' block at
    // the end of the function.
    BasicBlock *ThenBB = helper->createBBinFunc("then", TheFunction);
    BasicBlock *ElseBB = nullptr;
    if (node->elseSt != nullptr) {
        ElseBB = helper->createBBinFunc("else", TheFunction);
    }
    BasicBlock *MergeBB = helper->createBBinFunc("ifcont", TheFunction);

    if (node->elseSt != nullptr) {
        helper->createIfElse(cond, ThenBB, ElseBB);
    } else {
        helper->createIfElse(cond, ThenBB, MergeBB);
    }

    // Emit then value.
    helper->activateBB(ThenBB);
    bool br = genBlock(make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{node->thenSt}));
    if (!br) {
        helper->createBr(MergeBB);
    }
    // Codegen of 'Then' can change the current block, update ThenBB for the
    // PHI.
    ThenBB = helper->getActiveBB();

    if (node->elseSt != nullptr) {
        // Emit else block.
        helper->activateBB(ElseBB);
        br = genBlock(make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{node->elseSt}));
        if (!br) {
            helper->createBr(MergeBB);
        }
        // codegen of 'Else' can change the current block, update ElseBB for the
        // PHI.
        ElseBB = helper->getActiveBB();
    }

    helper->activateBB(MergeBB);
}

void CodeGen::genWhile(shared_ptr<IRWhile> node) {
    Function *TheFunction = helper->getCurrFunction();
    BasicBlock *beforeWhileBB = helper->createBBinFunc("beforewhile", TheFunction);
    BasicBlock *whileBB = helper->createBBinFunc("while", TheFunction);
    BasicBlock *whilebodyBB = helper->createBBinFunc("whilebody", TheFunction);
    BasicBlock *whilecontBB = helper->createBBinFunc("whilecont", TheFunction);

    helper->createBr(beforeWhileBB);
    helper->activateBB(beforeWhileBB);
    bool beforeBr = genBlock(make_shared<IRBlock>(node->before));
    if (!beforeBr) {
        helper->createBr(whileBB);
    }

    Value *cond = genExpression(node->cond, false);

    helper->createBr(whileBB);
    helper->activateBB(whileBB);

    helper->createIfElse(cond, whilebodyBB, whilecontBB);

    helper->activateBB(whilebodyBB);

    bool br = genBlock(make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{node->body}));
    if (!br) {
        helper->createBr(whilecontBB);
    }

    helper->activateBB(whilecontBB);
}

void CodeGen::genVarDecl(shared_ptr<IRVarDecl> node) {
    Value *ptr =
        helper->createAlloca(utils->getType(node->type), nullptr, node->name);
    NamedValues[node->name] = ptr;
    varTypes[node->name] = utils->getType(node->type);
    currBlockVars.top().push_back(
        pair<Value *, string>(ptr, node->type));
}

Value *CodeGen::genExpression(shared_ptr<IRExpression> node, bool genRefs) {

}