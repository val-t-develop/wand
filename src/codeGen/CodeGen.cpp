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
#include <IRTree/node/statement/IRVarsDecl.hpp>
#include <IRTree/node/statement/IRLoop.hpp>
#include <IRTree/node/statement/expression/IRAccess.hpp>
#include <IRTree/node/statement/expression/IRAlloc.hpp>
#include <IRTree/node/statement/expression/IRValue.hpp>
#include <IRTree/node/statement/expression/IRVar.hpp>
#include <ast/node/statement/expression/literal/StringLiteralNode.hpp>
#include <utils/UniqueNumber.hpp>

CodeGen::DestructAfterStatement::DestructAfterStatement(Value *_val,
                                                        string _type) : val(_val), type(_type) {}

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
        vector<Type *>{helper->getIntType(64)});
    helper->createFunctionPrototype(
        "__spl__destroyobj", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
            helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype(
        "__spl__destroyref", helper->getVoidType(),
        vector<Type *>{helper->getPointerType(helper->getVoidType()),
            helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype(
        "__spl__destroyref_not_delete", helper->getVoidType(),
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
        genFunctionPrototype(el.second);
    }
    for (auto el : tree->funcs) {
        genFunction(el.second);
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
        varTypes[var->name] = var->type;
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
    utils->functionTypes[node->name]=node->type;
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
            varTypes[node->args[i]->name] = node->args[i]->type;
        }

        bool br = genBlock(node->body);
        if (!br) {
            helper->createBr(retBB);
        }

        helper->activateBB(retBB);

        if (TheFunction->getReturnType() != helper->getVoidType()) {
            Value *ret_val = helper->createLoad(TheFunction->getReturnType(),
                                                ret_ptr, "retloadtmp");
            if (currFunction->type != "bool" && currFunction->type != "int" && currFunction->type != "byte" &&
                currFunction->type != "short" && currFunction->type != "long" && currFunction->type != "float" &&
                currFunction->type != "double" && currFunction->type != "char" && currFunction->type != "void") {
                helper->createCall("__spl__destroyref_not_delete", vector<Value*>{ret_ptr, helper->getFunction("__spl__destructor__" +
                                                    currFunction->type)});
            }
            helper->createRet(ret_val);
        } else {
            helper->createRet();
        }

        verifyFunction(*TheFunction);

        if (node->type == "int") {
            if (node->name.ends_with("main__spl__int__String")) {
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
        ret = genStatement(item, false);
        if (ret) {
            break;
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

bool CodeGen::genStatement(shared_ptr<IRStatement> node, bool makeRet) {
    bool ret = false;
    if (node != nullptr) {
        if (node->kind == IRNode::Kind::BLOCK) {
            ret = genBlock(static_pointer_cast<IRBlock>(node));
            utils->destructAfterStatement();
        } else if (node->kind == IRNode::Kind::RETURN) {
            auto returnNode = static_pointer_cast<IRReturn>(node);
            if (returnNode->val != nullptr) {
                Value *ptr =
                    NamedValues[helper->getCurrFunction()->getName().str() +
                                "__spl__ret"];
                Value *val = genExpression(returnNode->val, false);
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
        } else if (node->kind == IRNode::Kind::VAR_DECL) {
            genVarDecl(static_pointer_cast<IRVarDecl>(node));
            utils->destructAfterStatement();
        } else if (node->kind == IRNode::Kind::VARS_DECL) {
            for (auto el : static_pointer_cast<IRVarsDecl>(node)->vars) {
                genVarDecl(el);
            }
            utils->destructAfterStatement();
        } else if (node->isExpression()) {
            genExpression(static_pointer_cast<IRExpression>(node), false);
            utils->destructAfterStatement();
        } else if (node->kind == IRNode::Kind::IF_ELSE) {
            genIfElse(static_pointer_cast<IRIfElse>(node));
            utils->destructAfterStatement();
        } else if (node->kind == IRNode::Kind::LOOP) {
            genLoop(static_pointer_cast<IRLoop>(node));
            utils->destructAfterStatement();
        }
    }
    if (makeRet) {
        if (ret) {
            helper->createBr(retBB);
        }
    }
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

void CodeGen::genLoop(shared_ptr<IRLoop> node) {
    Function *TheFunction = helper->getCurrFunction();
    BasicBlock *beforeWhileBB = helper->createBBinFunc("beforewhile", TheFunction);
    BasicBlock *whileBB = helper->createBBinFunc("while", TheFunction);
    BasicBlock *whilebodyBB = helper->createBBinFunc("whilebody", TheFunction);
    BasicBlock *whilecontBB = helper->createBBinFunc("whilecont", TheFunction);

    helper->createBr(beforeWhileBB);
    helper->activateBB(beforeWhileBB);
    if (node->before!=nullptr) {
        genStatement(node->before, true);
    }

    helper->createBr(whileBB);
    helper->activateBB(whileBB);

    Value *cond = genExpression(node->cond, false);
    helper->createIfElse(cond, whilebodyBB, whilecontBB);

    helper->activateBB(whilebodyBB);

    bool br = genBlock(make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{node->body}));
    if (node->update!=nullptr) {
        genStatement(node->update, true);
    }
    if (!br) {
        helper->createBr(whileBB);
    }

    helper->activateBB(whilecontBB);
}

void CodeGen::genVarDecl(shared_ptr<IRVarDecl> node) {
    Value *ptr =
        helper->createAlloca(utils->getType(node->type), nullptr, node->name);
    NamedValues[node->name] = ptr;
    varTypes[node->name] = node->type;
    currBlockVars.top().push_back(
        pair<Value *, string>(ptr, node->type));
    if (node->init!=nullptr) {
        genExpression(make_shared<IRBinOp>(make_shared<IRVar>(node->name), node->init, BinaryOperatorNode::BinaryOperatorKind::ASSIGN), false);
    }
}

Value *CodeGen::genExpression(shared_ptr<IRExpression> node, bool genRef) {
    if (node->kind == IRNode::Kind::VAR) {
        if (genRef) {
            isRef = true;
        }
        return genVarValue(static_pointer_cast<IRVar>(node), genRef);
    } else if (node->kind == IRNode::Kind::ACCESS) {
        auto access = static_pointer_cast<IRAccess>(node);
        Value *val;
        if (access->access.size()==1) {
            val = genExpression(access->access[0], genRef);
        } else {
            val = genExpression(access->access[0], false);
        }
        string val_type;
        auto tmp_access = access;
        while (true) {
            if (tmp_access->access[0]->kind==IRNode::Kind::VAR) {
                val_type = varTypes[static_pointer_cast<IRVar>(tmp_access->access[0])->name];
            } else if (tmp_access->access[0]->kind==IRNode::Kind::ACCESS) {
                tmp_access = static_pointer_cast<IRAccess>(tmp_access->access[0]);
                continue;
            } else if (tmp_access->access[0]->kind == IRNode::Kind::CALL) {
                val_type = utils->functionTypes[static_pointer_cast<IRCall>(tmp_access->access[0])->name];
            } else {
                Out::errorMessage("BUG! Can not get expression kind!");
            }
            break;
        }
        // TODO array access

        for (int i = 1; i < access->access.size(); ++i) {
            if (access->access[i]->kind==IRNode::Kind::VAR) {
                int n = -1;
                shared_ptr<IRStruct> Struct;
                for (auto el : tree->structs) {
                    if (el->name==val_type) {
                        Struct=el;
                        break;
                    }
                }
                for (int j = 0; j < Struct->fields.size(); ++j) {
                    if (Struct->fields[j]->name==static_pointer_cast<IRVar>(access->access[i])->name) {
                        n=j;
                        break;
                    }
                }
                if (n==-1) {
                    Out::errorMessage("Can not find field in class");
                }
                vector<Value *> ids = vector<Value *>{
                    helper->getConstInt(32, 0), helper->getConstInt(32, n)};
                Value *getelementptr = helper->createGetElementPtr(
                    utils->getTypeNoPtr(val_type), val, ids);

                val_type=Struct->fields[n]->type;
                if (i==access->access.size()-1 && genRef) {
                    isRef = true;
                    val=getelementptr;
                } else {
                    val =
                        helper->createLoad(utils->getType(val_type), getelementptr);
                }
            } else if (access->access[i]->kind==IRNode::Kind::ACCESS) {
                Out::errorMessage("Something went wrorg when processing access!");
            } else if (access->access[i]->kind==IRNode::Kind::CALL) {
                Out::errorMessage("Something went wrorg when processing access!");
            }
        }
        return val;
    } else if (node->kind == IRNode::Kind::CALL) {
        if (genRef) {
            isRef = false;
        }
        return genCall(static_pointer_cast<IRCall>(node));
    } else if (node->kind == IRNode::Kind::BIN_OP) {
        if (genRef) {
            isRef = false;
        }
        return genBinOp(static_pointer_cast<IRBinOp>(node));
    } else if (node->kind == IRNode::Kind::UN_OP) {

    } else if (node->kind == IRNode::Kind::ALLOC) {
        Type *type = utils->getTypeNoPtr(static_pointer_cast<IRAlloc>(node)->type);
        Value *sizeofIV = helper->createSizeof(type);
        return helper->createCall("__spl__alloc", vector<Value *>{sizeofIV}, "heapallocatmp");
    } else if (node->kind == IRNode::Kind::FUNCTION_POINTER) {

    } else if (node->isLiteral()) {
        if (genRef) {
            isRef = false;
        }
        return genLiteral(static_pointer_cast<IRLiteral>(node));
    }
    return nullptr;
}

Value *CodeGen::genLiteral(shared_ptr<IRLiteral> node) {
    if (node->kind == IRNode::Kind::CHAR_LITERAL) {
        return helper->getConstInt(8, node->strLiteral[0]);
    } else if (node->kind == IRNode::Kind::BOOL_LITERAL) {
        return helper->getConstInt(1, node->boolLiteral);
    } else if (node->kind == IRNode::Kind::BYTE_LITERAL) {
        return helper->getConstInt(8, node->intLoteral);
    } else if (node->kind == IRNode::Kind::SHORT_LITERAL) {
        return helper->getConstInt(16, node->intLoteral);
    } else if (node->kind == IRNode::Kind::INT_LITERAL) {
        return helper->getConstInt(32, node->intLoteral);
    } else if (node->kind == IRNode::Kind::LONG_LITERAL) {
        return helper->getConstInt(64, node->intLoteral);
    } else if (node->kind == IRNode::Kind::FLOAT_LITERAL) {
        return helper->getConstFloat(node->doubleLiteral);
    } else if (node->kind == IRNode::Kind::DOUBLE_LITERAL) {
        return helper->getConstDouble(node->doubleLiteral);
    } else if (node->kind == IRNode::Kind::NULL_LITERAL) {
        return helper->getNullptr(
            helper->getPointerType(helper->getVoidType()));
    } else if (node->kind == IRNode::Kind::STRING_LITERAL) {
        Constant *c = helper->createConstantVar(
            helper->getArrayType(helper->getIntType(8),
                                 node->strLiteral.size()+1),
            "__spl__str__literal"+std::to_string(getNextUniqueNumber()),
            helper->getConstNullTerminatedString(node->strLiteral));
        auto v = helper->createCall(
            "__spl__constructor__String____StringLiteral", {c});
        destructAfterStatement.push_back(
            DestructAfterStatement(v, "String"));
        return v;
    }
}
Value *CodeGen::genCall(shared_ptr<IRCall> node) {
    vector<Value *> args = vector<Value *>();
    for (auto arg : node->args) {
        args.push_back(genExpression(arg, false));
    }
    auto tmp = helper->createCall(node->name, args);
    if (tmp->getType()->isPointerTy()) {
        destructAfterStatement.push_back(DestructAfterStatement(
            tmp, utils->functionTypes[node->name]));
    }
    return tmp;
}

Value *CodeGen::genVarValue(shared_ptr<IRVar> node, bool genRef) {
    Value *ptr = NamedValues[node->name];
    if (ptr == nullptr) {
        ptr = GlobalNamedValues[node->name];
    }
    Type *type = utils->getType(varTypes[node->name]);
    if (genRef) {
        return ptr;
    } else {
        Value *val = helper->createLoad(type, ptr);
        return val;
    }
}

Value *CodeGen::genBinOp(shared_ptr<IRBinOp> node) {
    if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::ASSIGN) {
        auto L = genExpression(node->left, true);
        auto R = genExpression(node->right, false);
        string type = node->left->getReturnType(this);
        if (type!="char"&&
            type!="bool"&&
            type!="byte"&&
            type!="short"&&
            type!="int"&&
            type!="long"&&
            type!="float"&&
            type!="double") {
            helper->createCall("__spl__addref", vector<Value *>{L, R, helper->getFunction("__spl__destructor__"+type)});
        }
        helper->createStore(R, L);
        return R;
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::BIT_OR_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::XOR_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::MOD_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::BIT_AND_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::DIV_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::MUL_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::SUB_ASSIGN) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::ADD_ASSIGN) {

    }
    auto L = genExpression(node->left, false);
    auto R = genExpression(node->right, false);
    if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::OR) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::AND) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::BIT_OR) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::XOR) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::BIT_AND) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPEQ(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createEQ(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPNE(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createNE(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::LESS) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPLT(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createLT(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::GREATER) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPGT(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createGT(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPLE(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createLE(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPGE(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createGE(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT) {

    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::ADD) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPAdd(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createAdd(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::SUB) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPSub(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createSub(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::MUL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPMul(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createMul(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::DIV) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPDiv(helper->castToDouble(L), helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createDiv(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->opKind==BinaryOperatorNode::BinaryOperatorKind::MOD) {

    }
    Out::errorMessage("BUG! Can not return value for binary operation");
    return nullptr;
}