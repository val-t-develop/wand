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

CodeGen::CodeGen(shared_ptr<CompilationUnitNode> _cu, Path& _file) : cu(_cu), file(_file), retBB(nullptr) {
    string moduleName = "__unnamedModule";
    if (!cu->nodes.empty()) {
        if (cu->nodes[0]->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
            moduleName = "";
            for (string str :
                 static_pointer_cast<PackageDeclNode>(cu->nodes[0])->name) {
                if (str != "") {
                    moduleName += str + ".";
                }
            }
            moduleName.pop_back();
        }
    }

    helper = make_shared<LLVMHelper>(moduleName);
    utils = make_shared<CodeGenUtils>(this);
}

void CodeGen::codeGen() {
    genImport(file);

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

    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::IMPORT_DECL_NODE) {
        } else if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            createClassType(static_pointer_cast<ClassDeclNode>(node));
        } else {
            Out::errorMessage("Can not generate this node");
        }
    }

    for (auto ps : imported) {
        Path p(ps);
        auto importCU = Main::CUs[p]->cu;

        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
                auto name = static_pointer_cast<PackageDeclNode>(n)->name;
                if (name.size()==2) {
                    if (name[0]=="spl" && name[1]=="core") {
                        if (p.getFilename() == "core.spl") {
                            auto dir = p.getParent();
                            auto ll_file = Path(dir.getName() + "/spl.core.stdlib.ll");
                            system(string("mkdir -p .spl_compilation"+dir.getName()).c_str());
                            string o_file = ".spl_compilation" + dir.getName() + "/spl.core.stdlib.o";
                            Main::currCUsStack.top()->linkingObj.push_back(o_file);
                            system(string("clang " + ll_file.getName() + " -c -o " + o_file).c_str());
                        }
                    }
                }
            } else if (n->kind == Node::NodeKind::IMPORT_DECL_NODE) {
                genImport(p);
            } else if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                createClassType(static_pointer_cast<ClassDeclNode>(n));
            } else {
                Out::errorMessage("Can not generate this node");
            }
        }
    }

    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            genStruct(static_pointer_cast<ClassDeclNode>(node));
        }
    }

    for (auto ps : imported) {
        Path p(ps);
        auto importCU = Main::CUs[p]->cu;

        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                genStruct(static_pointer_cast<ClassDeclNode>(n));
            }
        }
    }

    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::IMPORT_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            genClassDecl(static_pointer_cast<ClassDeclNode>(node), true);
        } else {
            Out::errorMessage("Can not generate this node");
        }
    }

    for (auto ps : imported) {
        Path p(ps);
        auto importCU = Main::CUs[p]->cu;

        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
                continue;
            } else if (n->kind == Node::NodeKind::IMPORT_DECL_NODE) {
                continue;
            } else if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                genClassDecl(static_pointer_cast<ClassDeclNode>(n), false);
            } else {
                Out::errorMessage("Can not generate this node");
            }
        }
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

void CodeGen::genImport(Path f) {
    vector<Path> importFiles{};
    for (auto imports : Main::CUs[f]->importFiles) {
        auto paths = imports.second;
        for (auto path : paths) {
            importFiles.push_back(path);
        }
    }

    for (auto p : importFiles) {
        bool alreadyImported = false;
        for (string importedOne : imported) {
            if (importedOne == p.getName()) {
                alreadyImported = true;
            }
        }
        if (alreadyImported) {
            continue;
        }
        imported.push_back(p.getName());
        Main::CUs[p]->completeToState(CU::State::AST);
    }
}

void CodeGen::createClassType(shared_ptr<ClassDeclNode> node) {
    classesStack.push(node);

    StructType *structType = helper->createStructType(classesStack.top()->getFullName());
    utils->classesTypes.insert({classesStack.top()->getFullName(), structType});

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        createClassType(item);
    }

    classesStack.pop();
}

void CodeGen::genClassDecl(shared_ptr<ClassDeclNode> node, bool genMethod) {
    classesStack.push(node);

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        genClassDecl(item, genMethod);
    }

    if (genMethod) {
        genDestructorDecl(node);
        for (shared_ptr<MethodDeclNode> item : node->methods) {
            genMethodDecl(item);
        }
        for (shared_ptr<ConstructorDeclNode> item : node->constructors) {
            genConstructorDecl(item);
        }
        if (node->constructors.empty()) {
            genConstructorDecl(make_shared<ConstructorDeclNode>(
                nullptr, nullptr, vector<shared_ptr<VarDeclNode>>{},
                make_shared<BlockNode>(vector<shared_ptr<Node>>{}, nullptr),
                nullptr));
        }
    }

    classesStack.pop();
}

void CodeGen::genStruct(shared_ptr<ClassDeclNode> node) {
    classesStack.push(node);

    vector<Type *> types = vector<Type *>();

    for (shared_ptr<VarDeclNode> var : node->fields) {
        types.push_back(utils->getType(var->type->getFullName()));
        varTypes[var->record] = utils->getType(var->type->getFullName());
        var->getFullName();

        bool isStatic = false;
        auto mods = var->modifiers->modifiers;
        for (auto mod : mods) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }

        if (isStatic) {
            GlobalVariable *ptr = helper->createGlobalVar(
                helper->getPointerType(utils->getType(var->type->getFullName())),
                var->getFullName());
            GlobalNamedValues[var->getFullName()] = ptr;
            ConstantPointerNull *const_ptr_2 =
                helper->getNullptr(helper->getPointerType(
                    utils->getType(var->type->getFullName())));
            ptr->setInitializer(const_ptr_2);
            StaticGlobalsInit[ptr] = var->init;
        }
    }
    string fullName = node->getFullName();

    StructType *structType;
    if (utils->classesTypes.contains(fullName)) {
        structType = utils->classesTypes.at(fullName);
    } else {
        structType = nullptr;
        Out::errorMessage("Can not get " + fullName);
    }
    structType->setBody(types);

    for (shared_ptr<MethodDeclNode> item : node->methods) {
        item->parent=node;
        genMethodPrototype(item);
    }

    for (shared_ptr<ConstructorDeclNode> item : node->constructors) {
        genConstructorPrototype(item);
    }
    if (node->constructors.empty()) {
        genConstructorPrototype(make_shared<ConstructorDeclNode>(
            nullptr, nullptr, vector<shared_ptr<VarDeclNode>>{}, nullptr,
            nullptr));
    }

    genDestructorPrototype(node);

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        genStruct(item);
    }
    classesStack.pop();
}

Function *CodeGen::genMethodPrototype(shared_ptr<MethodDeclNode> node) {
    vector<Type *> args_types = vector<Type *>();
    bool isStatic = false;
    for (auto mod : node->modifiers->modifiers) {
        if (mod == ModifiersNode::ModifierKind::STATIC) {
            isStatic = true;
            break;
        }
    }
    if (!isStatic) {
        for (int i = 0; i < node->args.size(); ++i) {
            if (i==0) {
                node->args[0]->record->typeRec=classesStack.top()->record;
                args_types.push_back(utils->getType(classesStack.top()->getFullName()));
            } else {
                auto arg = node->args[i];
                args_types.push_back(utils->getType(arg->type->getFullName()));
            }
        }
    } else {
        for (int i = 0; i < node->args.size(); ++i) {
            auto arg = node->args[i];
            args_types.push_back(utils->getType(arg->type->getFullName()));
        }
    }
    return helper->createFunctionPrototype(
        node->getFullName(), utils->getType(node->returnType->getFullName()),
        args_types);
}

Function *CodeGen::genDestructorPrototype(shared_ptr<ClassDeclNode> node) {
    vector<Type *> args_types = vector<Type *>();
    args_types.push_back(utils->getType(classesStack.top()->getFullName()));
    return helper->createFunctionPrototype("__spl__destructor__" +
                                               classesStack.top()->getFullName(),
                                           helper->getVoidType(), args_types);
}

Function *
CodeGen::genConstructorPrototype(shared_ptr<ConstructorDeclNode> node) {
    string str = "__spl__constructor__" + classesStack.top()->getFullName();
    for (auto arg : node->args) {
        str += "__" + arg->type->getFullName();
    }
    vector<Type *> args_types = vector<Type *>();
    for (shared_ptr<VarDeclNode> arg : node->args) {
        args_types.push_back(utils->getType(arg->type->getFullName()));
    }
    return helper->createFunctionPrototype(
        str, utils->getType(classesStack.top()->getFullName()), args_types);
}

Function *CodeGen::genMethodDecl(shared_ptr<MethodDeclNode> node) {
    currMethod= node;
    if (node->body != nullptr) {
        Function *TheFunction = helper->getFunction(node->getFullName());

        vector<Type *> args_types = vector<Type *>();
        bool isStatic = false;
        for (auto mod : node->modifiers->modifiers) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (!isStatic) {
            args_types.push_back(utils->getType(classesStack.top()->getFullName()));
            for (int i = 1; i < node->args.size(); ++i) {
                auto arg = node->args[i];
                args_types.push_back(utils->getType(arg->type->getFullName()));
            }
        } else {
            for (int i = 0; i < node->args.size(); ++i) {
                auto arg = node->args[i];
                args_types.push_back(utils->getType(arg->type->getFullName()));
            }
        }

        if (!TheFunction) {
            return nullptr;
        }

        BasicBlock *BB = helper->createBBinFunc("entry", TheFunction);
        retBB = helper->createBBinFunc("ret", TheFunction);
        helper->activateBB(BB);

        NamedValues.clear();

        Value *ret_ptr;
        if (TheFunction->getReturnType() != helper->getVoidType()) {
            ret_ptr = helper->createAlloca(TheFunction->getReturnType(),
                                           nullptr, "retallocatmp");
            NamedValues[node->getFullName() + "__spl__ret"] = ret_ptr;
            helper->createStore(genDefaultValue(node->returnType), ret_ptr);
        }
        for (int i = 0; i < node->args.size(); ++i) {
            auto Arg = TheFunction->getArg(i);
            if (isStatic) {
                string argName = node->args[i]->getFullName();
                Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
                helper->createStore(Arg, ptr);
                NamedValues[argName] = ptr;
                varTypes[node->args[i]->record] = Arg->getType();
            } else {
                if (i == 0) {
                    string argName = "this";
                    Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
                    helper->createStore(Arg, ptr);
                    NamedValues[argName] = ptr;

                    varTypes[node->args[0]->record] = Arg->getType();
                    thisRecord = node->args[0]->record;
                } else {
                    string argName = node->args[i]->getFullName();
                    Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
                    helper->createStore(Arg, ptr);
                    NamedValues[argName] = ptr;
                    varTypes[node->args[i]->record] = Arg->getType();
                }

            }
        }

        bool br = genBlockStatement(node->body);
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

        if (node->returnType->type->record->id == "int") {
            if (node->record->id == "main") {
                if (node->args.size() == 1) {
                    if (node->args[0]->type->type->record->id == "String") {
                        Function *MainFunction =
                            helper->createFunctionPrototype(
                                "main", helper->getIntType(32),
                                vector<Type *>());

                        if (!MainFunction) {
                            return nullptr;
                        }

                        BasicBlock *mainBB =
                            helper->createBBinFunc("entry", MainFunction);
                        helper->activateBB(mainBB);

                        helper->createCall("__spl__init__gc");

                        for (auto globInit : StaticGlobalsInit) {
                            if (globInit.second != nullptr) {
                                Value *ptr = globInit.first;
                                Value *val = genExpression(globInit.second, false);
                                helper->createStore(val, ptr);
                            }
                        }

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

        return TheFunction;
    }
    return nullptr;
}

Function *CodeGen::genDestructorDecl(shared_ptr<ClassDeclNode> node) {
    if (!node->destructors.empty()) {
        if (node->destructors[0]->body!=nullptr) {
            Function *TheFunction =
                helper->getFunction("__spl__destructor__" + classesStack.top()->getFullName());

            vector<Type *> args_types = vector<Type *>();
            args_types.push_back(utils->getType(classesStack.top()->getFullName()));

            if (!TheFunction) {
                return nullptr;
            }

            BasicBlock *BB = helper->createBBinFunc("entry", TheFunction);
            retBB = helper->createBBinFunc("ret", TheFunction);
            helper->activateBB(BB);

            NamedValues.clear();

            genBlockStatement(node->destructors[0]->body);

            Value *val = TheFunction->getArg(0);
            for (int i = 0; i < node->fields.size(); ++i) {
                Value *ptr = helper->createGetElementPtr(
                    utils->getTypeNoPtr(node->getFullName()), val,
                    vector<Value *>{helper->getConstInt(32, 0),
                                    helper->getConstInt(32, i)});
                if (node->fields[i]->type->type->record->type != "primitive") {
                    helper->createCall(
                        "__spl__destroyref",
                        vector<Value *>{
                            ptr,
                            helper->getFunction("__spl__destructor__" +
                                                node->fields[i]->type->getFullName())});
                }
            }

            helper->createBr(retBB);

            helper->activateBB(retBB);

            helper->createRet(UndefValue::get(helper->getVoidType()));

            verifyFunction(*TheFunction);

            return TheFunction;
        }
        return nullptr;
    } else {
        Function *TheFunction =
            helper->getFunction("__spl__destructor__" + classesStack.top()->getFullName());

        vector<Type *> args_types = vector<Type *>();
        args_types.push_back(utils->getType(classesStack.top()->getFullName()));

        if (!TheFunction) {
            return nullptr;
        }

        BasicBlock *BB = helper->createBBinFunc("entry", TheFunction);
        retBB = helper->createBBinFunc("ret", TheFunction);
        helper->activateBB(BB);

        NamedValues.clear();

        Value *val = TheFunction->getArg(0);
        for (int i = 0; i < node->fields.size(); ++i) {
            Value *ptr = helper->createGetElementPtr(
                utils->getTypeNoPtr(node->getFullName()), val,
                vector<Value *>{helper->getConstInt(32, 0),
                                helper->getConstInt(32, i)});
            if (node->fields[i]->type->type->record->type != "primitive") {
                helper->createCall(
                    "__spl__destroyref",
                    vector<Value *>{
                        ptr,
                        helper->getFunction("__spl__destructor__" +
                                            node->fields[i]->type->getFullName())});
            }
        }

        helper->createBr(retBB);

        helper->activateBB(retBB);

        helper->createRet(UndefValue::get(helper->getVoidType()));

        verifyFunction(*TheFunction);

        return TheFunction;
    }

}

Function *CodeGen::genConstructorDecl(shared_ptr<ConstructorDeclNode> node) {
    if (node->body != nullptr) {
        string str = "__spl__constructor__" + classesStack.top()->getFullName();
        for (auto arg : node->args) {
            str += "__" + arg->type->getFullName();
        }
        Function *TheFunction = helper->getFunction(str);

        vector<Type *> args_types = vector<Type *>();
        for (shared_ptr<VarDeclNode> arg : node->args) {
            args_types.push_back(utils->getType(arg->type->getFullName()));
        }
        if (!TheFunction) {
            return nullptr;
        }

        BasicBlock *BB = helper->createBBinFunc("entry", TheFunction);
        helper->activateBB(BB);

        NamedValues.clear();

        for (int i = 0; i < node->args.size(); ++i) {
            auto Arg = TheFunction->getArg(i);
            string argName = node->args[i]->getFullName();
            Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
            helper->createStore(Arg, ptr);
            NamedValues[argName] = ptr;
            varTypes[node->args[i]->record] = Arg->getType();
        }
        genConstructorBlockStatement(node);

        verifyFunction(*TheFunction);
        return TheFunction;
    }
    return nullptr;
}

bool CodeGen::genBlockStatement(shared_ptr<BlockNode> node) {
    bool ret = false;
    currBlockVars.push(vector<pair<Value *, string>>());
    for (shared_ptr<Node> item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == Node::NodeKind::BLOCK_NODE) {
                ret = genBlockStatement(static_pointer_cast<BlockNode>(item));
                utils->destructAfterStatement();
                break;
            } else if (item->kind == Node::NodeKind::RETURN_NODE) {
                if (static_pointer_cast<ReturnNode>(item)->expression !=
                    nullptr) {
                    Value *ptr =
                        NamedValues[helper->getCurrFunction()->getName().str() +
                                    "__spl__ret"];
                    Value *val = genExpression(
                        static_pointer_cast<ReturnNode>(item)->expression, false);
                    if (val->getType()->isPointerTy()) {
                        helper->createCall("__spl__addref",
                                       vector<Value *>{ptr, val, helper->getFunction("__spl__destructor__"+currMethod->returnType->getFullName())});
                    }
                    helper->createStore(val, ptr);
                }
                ret = true;
                utils->destructAfterStatement();
                break;
            } else if (item->kind == Node::NodeKind::VAR_DECL_NODE) {
                genVarDecl(static_pointer_cast<VarDeclNode>(item));
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::VARS_DECL_NODE) {
                for (shared_ptr<VarDeclNode> decl :
                     static_pointer_cast<VarsDeclNode>(item)->decls) {
                    genVarDecl(decl);
                }
                utils->destructAfterStatement();
            } else if (item->isExpression()) {
                genExpression(static_pointer_cast<ExpressionNode>(item), false);
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::IF_ELSE_NODE) {
                genIfElse(static_pointer_cast<IfElseNode>(item));
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::WHILE_NODE) {
                genWhile(static_pointer_cast<WhileNode>(item));
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::FOR_NODE) {
                genFor(static_pointer_cast<ForNode>(item));
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
                vector<Value *>{v.first, helper->getFunction("__spl__destructor__" +
                                                         v.second)});
        }
    }
    if (ret) {
        helper->createBr(retBB);
    }
    currBlockVars.pop();
    return ret;
}

bool CodeGen::genConstructorBlockStatement(
    shared_ptr<ConstructorDeclNode> constructor) {
    shared_ptr<BlockNode> node = constructor->body;
    currBlockVars.push(vector<pair<Value *, string>>());

    Type *type = utils->getTypeNoPtr(classesStack.top()->getFullName());
    Value *sizeofIV = helper->createSizeof(type);

    Value *heapallocatmp = helper->createCall(
        "__spl__alloc", vector<Value *>{sizeofIV}, "heapallocatmp");
    Value *ptr =
        helper->createAlloca(heapallocatmp->getType(), nullptr,
                             string(heapallocatmp->getName()) + "tmp_var");
    helper->createStore(heapallocatmp, ptr);

    for (int i = 0; i < classesStack.top()->fields.size(); i++) {
        shared_ptr<VarRecord> n_var_rec = classesStack.top()->fields[i]->record;
        auto classRecord = classesStack.top()->record;
        Type *t = utils->getTypeNoPtr(classRecord->getFullName());
        int struct_n = 0;
        for (int j = 0; j < classRecord->fields.size(); ++j) {
            if (classRecord->fields[j]->equals(n_var_rec)) {
                struct_n = j;
                break;
            }
        }
        Value *nullV = helper->getConstInt(32, 0);
        Value *struct_nV = helper->getConstInt(32, struct_n);
        Value *getelementptr = helper->createGetElementPtr(
            t, heapallocatmp, vector<Value *>{nullV, struct_nV}, "access_tmp");

        auto n_classRecord = classesStack.top()->fields[i]->type->type->record;
        Type *n_t = utils->getTypeNoPtr(n_classRecord->getFullName());
        auto last =
            helper->createLoad(n_t, getelementptr, "loadgetelementptrtmp");
        auto last_ptr = getelementptr;

        helper->createStore(
            genDefaultValue(make_shared<TypeNode>(
                make_shared<ClassRecordNode>(
                    n_classRecord, vector<shared_ptr<AccessNode>>(), nullptr),
                0, nullptr)),
            last_ptr);
    }

    for (shared_ptr<Node> item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == Node::NodeKind::BLOCK_NODE) {
                return genBlockStatement(static_pointer_cast<BlockNode>(item));
            } else if (item->kind == Node::NodeKind::RETURN_NODE) {
                if (static_pointer_cast<ReturnNode>(item)->expression !=
                    nullptr) {
                    Value *val = genExpression(
                        static_pointer_cast<ReturnNode>(item)->expression, false);
                    Value *ptr =
                        NamedValues[helper->getCurrFunction()->getName().str() +
                                    "__spl__ret"];
                    helper->createStore(val, ptr);
                }
                helper->createBr(retBB);
                return true;
            } else if (item->kind == Node::NodeKind::VAR_DECL_NODE) {
                genVarDecl(static_pointer_cast<VarDeclNode>(item));
            } else if (item->kind == Node::NodeKind::VARS_DECL_NODE) {
                for (shared_ptr<VarDeclNode> decl :
                     static_pointer_cast<VarsDeclNode>(item)->decls) {
                    genVarDecl(decl);
                }
            } else if (item->isExpression()) {
                genExpression(static_pointer_cast<ExpressionNode>(item), false);
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::IF_ELSE_NODE) {
                genIfElse(static_pointer_cast<IfElseNode>(item));
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::WHILE_NODE) {
                genWhile(static_pointer_cast<WhileNode>(item));
                utils->destructAfterStatement();
            } else if (item->kind == Node::NodeKind::FOR_NODE) {
                genFor(static_pointer_cast<ForNode>(item));
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
                vector<Value *>{v.first, helper->getFunction("__spl__destructor__" +
                                                         v.second)});
        }
    }
    currBlockVars.pop();

    helper->createRet(heapallocatmp);
    return false;
}

void CodeGen::genIfElse(shared_ptr<IfElseNode> node) {
    Value *cond = genExpression(node->condition, false);

    Function *TheFunction = helper->getCurrFunction();

    // Create blocks for the then and else cases.  Insert the 'then' block at
    // the end of the function.
    BasicBlock *ThenBB = helper->createBBinFunc("then", TheFunction);
    BasicBlock *ElseBB = nullptr;
    if (node->elseNode != nullptr) {
        ElseBB = helper->createBBinFunc("else", TheFunction);
    }
    BasicBlock *MergeBB = helper->createBBinFunc("ifcont", TheFunction);

    if (node->elseNode != nullptr) {
        helper->createIfElse(cond, ThenBB, ElseBB);
    } else {
        helper->createIfElse(cond, ThenBB, MergeBB);
    }

    // Emit then value.
    helper->activateBB(ThenBB);
    bool br = genBlockStatement(make_shared<BlockNode>(
        vector<shared_ptr<Node>>{node->thenNode}, nullptr));
    if (!br) {
        helper->createBr(MergeBB);
    }
    // Codegen of 'Then' can change the current block, update ThenBB for the
    // PHI.
    ThenBB = helper->getActiveBB();

    if (node->elseNode != nullptr) {
        // Emit else block.
        helper->activateBB(ElseBB);
        br = genBlockStatement(make_shared<BlockNode>(
            vector<shared_ptr<Node>>{node->elseNode}, nullptr));
        if (!br) {
            helper->createBr(MergeBB);
        }
        // codegen of 'Else' can change the current block, update ElseBB for the
        // PHI.
        ElseBB = helper->getActiveBB();
    }

    helper->activateBB(MergeBB);
}

void CodeGen::genWhile(shared_ptr<WhileNode> node) {
    Value *cond = genExpression(node->expression, false);

    Function *TheFunction = helper->getCurrFunction();

    BasicBlock *whileBB = helper->createBBinFunc("while", TheFunction);
    BasicBlock *whilebodyBB = helper->createBBinFunc("whilebody", TheFunction);
    BasicBlock *whilecontBB = helper->createBBinFunc("whilecont", TheFunction);

    helper->createBr(whileBB);

    helper->activateBB(whileBB);

    helper->createIfElse(cond, whilebodyBB, whilecontBB);

    helper->activateBB(whilebodyBB);

    bool br = genBlockStatement(make_shared<BlockNode>(
        vector<shared_ptr<Node>>{node->statement}, nullptr));
    if (!br) {
        helper->createBr(whileBB);
    }

    helper->activateBB(whilecontBB);
}

void CodeGen::genFor(shared_ptr<ForNode> node) {

    Function *TheFunction = helper->getCurrFunction();

    BasicBlock *forBB = helper->createBBinFunc("for", TheFunction);
    BasicBlock *forcondBB = helper->createBBinFunc("forcond", TheFunction);
    BasicBlock *forbodyBB = helper->createBBinFunc("forbody", TheFunction);
    BasicBlock *forcontBB = helper->createBBinFunc("forcont", TheFunction);

    helper->createBr(forBB);

    helper->activateBB(forBB);

    bool br = genBlockStatement(
        make_shared<BlockNode>(vector<shared_ptr<Node>>{node->init}, nullptr));
    if (br) {
        return;
    }
    helper->createBr(forcondBB);

    helper->activateBB(forcondBB);

    Value *cond = genExpression(node->condition, false);
    helper->createIfElse(cond, forbodyBB, forcontBB);

    helper->activateBB(forbodyBB);

    br = genBlockStatement(make_shared<BlockNode>(
        vector<shared_ptr<Node>>{node->statement}, nullptr));
    if (!br) {
        br = genBlockStatement(make_shared<BlockNode>(
            vector<shared_ptr<Node>>{node->update}, nullptr));
        if (br) {
            return;
        }
        helper->createBr(forcondBB);
    }
    // TODO br forcont
    helper->activateBB(forcontBB);
}

Value *CodeGen::genExpression(shared_ptr<ExpressionNode> node, bool genRef = false) {
    if (node->isLiteral()) {
        if (genRef) {
            isRef = false;
        }
        return genLiteral(node);
    } else if (node->kind == Node::NodeKind::METHOD_CALL_NODE) {
        if (genRef) {
            isRef = false;
        }
        shared_ptr<MethodCallNode> callNode =
            static_pointer_cast<MethodCallNode>(node);
        bool isStatic = false;
        for (auto mod : callNode->record->mods) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (isStatic) {
            return genMethodCall(callNode, nullptr);
        } else {
            Value *calle = nullptr;
            if (thisV.empty()) {
                Out::errorMessage("Error: can not generate this value.");
            } else {
                calle = thisV.top();
            }
            return genMethodCall(callNode, calle);
        }
    } else if (node->kind == Node::NodeKind::BINARY_OPERATOR_NODE) {
        if (genRef) {
            isRef = false;
        }
        return genBinOp(static_pointer_cast<BinaryOperatorNode>(node));
    } else if (node->kind == Node::NodeKind::VAR_RECORD_NODE) {
        if (genRef) {
            isRef = true;
        }
        return genVarValue(static_pointer_cast<VarRecordNode>(node), genRef);
    } else if (node->kind == Node::NodeKind::NEW_NODE) {
        if (genRef) {
            isRef = false;
        }
        return genNewNode(static_pointer_cast<NewNode>(node));
    } else if (node->kind == Node::NodeKind::ARRAY_CREATION_NODE) {
        if (genRef) {
            isRef = false;
        }
        auto n = static_pointer_cast<ArrayCreationNode>(node);
        if (n->isStatic) {
            Type *lastTy = utils->getType(n->type->getFullName());
            Value *mul = helper->getConstInt(64, 1);
            for (int i = n->dims.size() - 1; i >= 0; --i) {
                if (n->dims[i]->kind == Node::NodeKind::INT_LITERAL_NODE) {
                    lastTy = helper->getArrayType(
                        lastTy,
                        static_pointer_cast<IntLiteralNode>(n->dims[i])->value);
                } else {
                    Value *expr = genExpression(n->dims[i]);
                    if (expr->getType()->isIntegerTy()) {
                        if (expr->getType()->getPrimitiveSizeInBits() < 64) {
                            expr = CastInst::CreateZExtOrBitCast(
                                expr, helper->getIntType(64));
                        }
                    } else {
                        Out::errorMessage("Array size is not int type!");
                    }
                    mul = helper->createMul(mul, expr);
                }
            }
            Value *alloca = helper->createAlloca(lastTy, mul);
            return alloca;
        } else {
            Out::errorMessage("No dynamic arrays support yet.");
        }
    } else if (node->kind == Node::NodeKind::ARRAY_INITIALIZER_NODE) {

    } else if (node->kind == Node::NodeKind::ARRAY_ACCESS_NODE) {
        if (genRef) {
            isRef = false;
        }
        auto n = static_pointer_cast<ArrayAccessNode>(node);
        Value *arr = genExpression(n->array);
        // TODO
    } else if (node->kind == Node::NodeKind::ACCESS_NODE) {
        shared_ptr<AccessNode> access = static_pointer_cast<AccessNode>(node);
        Value *val = genExpression(
            static_pointer_cast<ExpressionNode>(access->access[0]));
        shared_ptr<ClassRecord> val_type = nullptr;
        if (access->access[0]->kind == Node::NodeKind::VAR_RECORD_NODE) {
            val_type = static_pointer_cast<VarRecordNode>(access->access[0])
                           ->getReturnType();
        } else if (access->access[0]->kind ==
                   Node::NodeKind::METHOD_CALL_NODE) {
            val_type = static_pointer_cast<MethodCallNode>(access->access[0])
                           ->getReturnType();
        } else if (access->access[0]->kind ==
                   Node::NodeKind::ARRAY_ACCESS_NODE) {
            // TODO
            // val_type =
            // static_pointer_cast<MethodCallNode>(access->access[0])->getReturnType();
        } else {
            Out::errorMessage("Can not generate expression");
        }
        for (int i = 1; i < access->access.size(); ++i) {
            if (access->access[i]->kind == Node::NodeKind::VAR_RECORD_NODE) {
                int n = -1;
                for (int j = 0; j < val_type->fields.size(); ++j) {
                    if (val_type->fields[j]->id ==
                        static_pointer_cast<VarRecordNode>(access->access[i])
                            ->record->id) {
                        n = j;
                        break;
                    }
                }
                if (n == -1) {
                    Out::errorMessage("Can not find field in class");
                }
                vector<Value *> ids = vector<Value *>{
                    helper->getConstInt(32, 0), helper->getConstInt(32, n)};
                Value *getelementptr = helper->createGetElementPtr(
                    utils->getTypeNoPtr(val_type->getFullName()), val, ids);
                val_type = static_pointer_cast<VarRecordNode>(access->access[i])
                               ->getReturnType();
                if (i==access->access.size()-1 && genRef) {
                    isRef = true;
                    val=getelementptr;
                } else {
                    val =
                        helper->createLoad(utils->getType(val_type->getFullName()), getelementptr);
                }
            } else if (access->access[i]->kind ==
                       Node::NodeKind::METHOD_CALL_NODE) {
                if (genRef) {
                    isRef = false;
                }
                val = genMethodCall(static_pointer_cast<MethodCallNode>(access->access[i]), val);
                val_type = static_pointer_cast<MethodCallNode>(access->access[i])->getReturnType();
            }
        }
        return val;
    }
    return nullptr;
}

Value *CodeGen::genLiteral(shared_ptr<ExpressionNode> node) {
    if (node->kind == Node::NodeKind::INT_LITERAL_NODE) {
        auto IntLiteral = static_pointer_cast<IntLiteralNode>(node);
        if (IntLiteral->type==IntLiteralNode::Type::BYTE) {
            return helper->getConstInt(
                8, static_pointer_cast<IntLiteralNode>(node)->value);
        } else if (IntLiteral->type==IntLiteralNode::Type::SHORT) {
            return helper->getConstInt(
                16, static_pointer_cast<IntLiteralNode>(node)->value);
        } else if (IntLiteral->type==IntLiteralNode::Type::INT) {
            return helper->getConstInt(
                32, static_pointer_cast<IntLiteralNode>(node)->value);
        } else if (IntLiteral->type==IntLiteralNode::Type::LONG) {
            return helper->getConstInt(
                64, static_pointer_cast<IntLiteralNode>(node)->value);
        }
    } else if (node->kind == Node::NodeKind::BOOL_LITERAL_NODE) {
        return helper->getConstInt(
            1, static_pointer_cast<BoolLiteralNode>(node)->value);
    } else if (node->kind == Node::NodeKind::CHAR_LITERAL_NODE) {
        return helper->getConstInt(
            8, static_pointer_cast<CharLiteralNode>(node)->str[0]);
    } else if (node->kind == Node::NodeKind::FLOAT_LITERAL_NODE) {
        if (static_pointer_cast<FloatLiteralNode>(node)->doubleVal) {
            return helper->getConstDouble(
                static_pointer_cast<FloatLiteralNode>(node)->value);
        } else {
            return helper->getConstFloat(
                static_pointer_cast<FloatLiteralNode>(node)->value);
        }
    } else if (node->kind == Node::NodeKind::STRING_LITERAL_NODE) {
        auto literal_node = std::static_pointer_cast<StringLiteralNode>(node);
        Constant *c = helper->createConstantVar(helper->getArrayType(helper->getIntType(8),
            literal_node->str.size()+1),"__spl__str__literal",
            helper->getConstNullTerminatedString(literal_node->str));
        auto v = helper->createCall("__spl__constructor__String____StringLiteral", {c});
        destructAfterStatement.push_back(DestructAfterStatement(v, "String", false));
        return v;
    }
    Out::errorMessage("BUG! Can not return value of literal");
    return nullptr;
}

Value *CodeGen::genMethodCall(shared_ptr<MethodCallNode> node, Value *calle) {
    if (node->record->similar.empty()) {
        vector<Value *> args = vector<Value *>();
        if (calle != nullptr) {
            args.push_back(calle);
            thisV.push(calle);
        }
        for (shared_ptr<ExpressionNode> arg : node->args) {
            args.push_back(genExpression(arg));
        }
        auto tmp = helper->createCall(node->getFullName(), args);
        if (calle != nullptr) {
            thisV.pop();
        }
        if (tmp->getType()->isPointerTy()) {
            destructAfterStatement.push_back(DestructAfterStatement(tmp, node->getReturnType()->getFullName(), true));
        }
        return tmp;
    } else {
        vector<Value *> args = vector<Value *>();
        if (calle != nullptr) {
            args.push_back(calle);
            thisV.push(calle);
        }
        for (shared_ptr<ExpressionNode> arg : node->args) {
            args.push_back(genExpression(arg));
        }
        if (calle!=nullptr) {
            node->args.insert(node->args.begin(), make_shared<VarRecordNode>(nullptr, nullptr));
        }

        auto funcsRecord = node->record->similar;
        for (auto funRecord : funcsRecord) {
            Function *TheFunction =
                helper->getFunction(funRecord->getFullName());
            bool same = true;
            for (int i = (calle!=nullptr?1:0); i < node->args.size(); ++i) {
                if (funRecord->vars[i]->typeRec->getFullName() != node->args[i]->getReturnType()->getFullName()) {
                    same = false;
                }
            }
            if (same) {
                auto tmp = helper->createCall(TheFunction, args);
                if (calle != nullptr) {
                    thisV.pop();
                }
                if (tmp->getType()->isPointerTy()) {
                    destructAfterStatement.push_back(DestructAfterStatement(tmp, funRecord->retTypeRec->getFullName(), true));
                }
                return tmp;
            }
        }
        auto funRecord = node->record;
        string fullName = funRecord->getFullName();
        Function *TheFunction = helper->getFunction(fullName);
        bool same = true;
        for (int i = (calle!=nullptr?1:0); i < args.size(); ++i) {
            if (funRecord->vars[i]->typeRec->getFullName() != node->args[i]->getReturnType()->getFullName()) {
                same = false;
            }
        }
        if (same) {
            auto tmp = helper->createCall(TheFunction, args);
            if (calle != nullptr) {
                thisV.pop();
            }
            if (tmp->getType()->isPointerTy()) {
                destructAfterStatement.push_back(DestructAfterStatement(tmp, funRecord->retTypeRec->getFullName(), true));
            }
            return tmp;
        }

        Out::errorMessage("Fatal error! Can not car record of method call. "
                          "Undefined reference: " +
                          node->record->id);
    }
    Out::errorMessage("BUG! Can not return value of method call");
    return nullptr;
}

Value *CodeGen::genVarDecl(shared_ptr<VarDeclNode> node) {
    Value *val = node->init != nullptr ? genExpression(node->init)
                                       : genDefaultValue(node->type);
    Value *ptr =
        helper->createAlloca(val->getType(), nullptr, node->getFullName());
    if (val->getType()->isPointerTy()) {
        helper->createCall("__spl__addref", vector<Value *>{ptr, val, helper->getFunction("__spl__destructor__"+node->type->getFullName())});
    }
    helper->createStore(val, ptr);
    NamedValues[node->getFullName()] = ptr;
    varTypes[node->record] = val->getType();
    currBlockVars.top().push_back(
        pair<Value *, string>(ptr, node->type->getFullName()));
    return ptr;
}

Value *CodeGen::genDefaultValue(shared_ptr<TypeNode> node) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "bool") {
            return helper->getConstInt(32, 0);
        } else if (x->record->id == "int") {
            return helper->getConstInt(32, 0);
        } else if (x->record->id == "byte") {
            return helper->getConstInt(8, 0);
        } else if (x->record->id == "short") {
            return helper->getConstInt(16, 0);
        } else if (x->record->id == "long") {
            return helper->getConstInt(64, 0);
        } else if (x->record->id == "float") {
            return helper->getConstFloat(0);
        } else if (x->record->id == "double") {
            return helper->getConstDouble(0);
        } else if (x->record->id == "char") {
            return helper->getConstInt(8, 0);
        } else if (x->record->id == "void") {
            return nullptr;
        }
    } else {
        StructType *structType;
        if (utils->classesTypes.contains(x->getFullName())) {
            structType = utils->classesTypes.at(x->getFullName());
        } else {
            structType = nullptr;
            Out::errorMessage("Can not get " + x->getFullName());
        }
        PointerType *structPtrType = helper->getPointerType(structType);
        return helper->getNullptr(structPtrType);
    }
    Out::errorMessage("BUG! Can not return default value");
    return nullptr;
}

Value *CodeGen::genVarValue(shared_ptr<VarRecordNode> node, bool genRef = false) {
    Value *ptr = NamedValues[node->getFullName()];
    if (ptr == nullptr) {
        ptr = GlobalNamedValues[node->getFullName()];
    }
    if (ptr == nullptr) {
        if (NamedValues["this"] != nullptr) {
            auto access = make_shared<AccessNode>(nullptr);
            access->access.push_back(make_shared<VarRecordNode>(thisRecord ,nullptr));
            access->access.push_back(node);
            return genExpression(access);
        }
    }
    Type *type = varTypes[node->record];
    if (genRef) {
        return ptr;
    } else {
        Value *val = helper->createLoad(type, ptr);
        return val;
    }
}

Value *CodeGen::genBinOp(shared_ptr<BinaryOperatorNode> node) {
    auto L = genExpression(node->left);
    auto R = genExpression(node->right, true);

    if (node->op ==
        BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::BIT_OR_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::BIT_AND_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ASSIGN) {
        if (node->left->kind == Node::NodeKind::ACCESS_NODE) {
            shared_ptr<AccessNode> access =
                static_pointer_cast<AccessNode>(node->left);
            Value *ptr = nullptr;
            Value *val = nullptr;
            shared_ptr<ClassRecord> val_type = nullptr;
            if (access->access[0]->kind == Node::NodeKind::VAR_RECORD_NODE) {
                shared_ptr<VarRecordNode> var =
                    static_pointer_cast<VarRecordNode>(access->access[0]);
                ptr = NamedValues[var->getFullName()];
                if (ptr == nullptr) {
                    ptr = GlobalNamedValues[var->getFullName()];
                }
                if (ptr == nullptr) {
                    if (NamedValues["this"] != nullptr) {
                        auto newAccess = make_shared<AccessNode>(nullptr);
                        access->access.emplace(access->access.begin(), make_shared<VarRecordNode>(thisRecord ,nullptr));
                        return genExpression(make_shared<BinaryOperatorNode>(access, node->right, node->op, node->parent));
                    }
                }
                Type *type = varTypes[var->record];
                val = helper->createLoad(type, ptr);
                val_type = var->getReturnType();
            } else if (access->access[0]->kind ==
                       Node::NodeKind::METHOD_CALL_NODE) {
                Out::errorMessage("Can not assign to method call result");
            } else {
                Out::errorMessage("Can not generate expression");
            }
            for (int i = 1; i < access->access.size(); ++i) {
                if (access->access[i]->kind ==
                    Node::NodeKind::VAR_RECORD_NODE) {
                    int n = -1;
                    for (int j = 0; j < val_type->fields.size(); ++j) {
                        if (val_type->fields[j]->id ==
                            static_pointer_cast<VarRecordNode>(
                                access->access[i])
                                ->record->id) {
                            n = j;
                            break;
                        }
                    }
                    if (n == -1) {
                        Out::errorMessage("Can not find field in class");
                    }
                    vector<Value *> ids = vector<Value *>{
                        helper->getConstInt(32, 0), helper->getConstInt(32, n)};
                    Value *getelementptr = helper->createGetElementPtr(
                        utils->getTypeNoPtr(val_type->getFullName()), val, ids);
                    val_type =
                        static_pointer_cast<VarRecordNode>(access->access[i])
                            ->getReturnType();
                    val = helper->createLoad(utils->getType(val_type->getFullName()),
                                             getelementptr);
                    ptr = getelementptr;
                } else if (access->access[i]->kind ==
                           Node::NodeKind::METHOD_CALL_NODE) {
                    // TODO
                }
            }
            if (R->getType()->isPointerTy()) {
                if (isRef) {
                    helper->createCall("__spl__write", vector<Value *>{ptr, R, helper->getFunction("__spl__destructor__"+val_type->getFullName())});
                } else {
                    helper->createCall("__spl__addref", vector<Value *>{ptr, R, helper->getFunction("__spl__destructor__"+val_type->getFullName())});
                }
            }
            helper->createStore(R, ptr);
            return R;
        } else {
            Out::errorMessage("Can not create assign.");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::OR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::AND) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_OR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_AND) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPEQ(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createEQ(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPNE(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createNE(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPLT(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createLT(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPGT(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createGT(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPLE(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createLE(L, R);
        }
    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPGE(helper->castToDouble(L),
                                      helper->castToDouble(R));
        } else {
            return helper->createGE(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD) {
        Type *StringPtrType = utils->getType("String");
        if (L->getType() == StringPtrType || R->getType() == StringPtrType) {
            if (L->getType() == StringPtrType) {
                if (R->getType() == StringPtrType) {
                    auto tmp = helper->createCall("String.concat__spl__String__String__String", {L, R});
                    destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                    return tmp;
                } else if (!R->getType()->isStructTy()) {
                    Type *charType = utils->getType("char");
                    Type *boolType = utils->getType("bool");
                    Type *byteType = utils->getType("byte");
                    Type *shortType = utils->getType("short");
                    Type *intType = utils->getType("int");
                    Type *longType = utils->getType("long");
                    Type *floatType = utils->getType("float");
                    Type *doubleType = utils->getType("double");
                    if (R->getType()==charType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__char", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==boolType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__bool", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==byteType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__byte", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==shortType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__short", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==intType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__int", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==longType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__long", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==floatType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__float", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (R->getType()==doubleType) {
                        auto tmp = helper->createCall("String.concat__spl__String__String__double", {L, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else {
                        Out::errorMessage("Can not get primitive type");
                    }
                } else {
                    Out::errorMessage("Can not concat class to string");
                }
            } else {
                if (!L->getType()->isStructTy()) {
                    Type *charType = utils->getType("char");
                    Type *boolType = utils->getType("bool");
                    Type *byteType = utils->getType("byte");
                    Type *shortType = utils->getType("short");
                    Type *intType = utils->getType("int");
                    Type *longType = utils->getType("long");
                    Type *floatType = utils->getType("float");
                    Type *doubleType = utils->getType("double");
                    if (L->getType()==charType) {
                        auto Str = helper->createCall("__spl__constructor__String__char", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (L->getType()==boolType) {
                        auto Str = helper->createCall("__spl__constructor__String__bool", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (L->getType()==byteType) {
                        auto Str = helper->createCall("__spl__constructor__String__byte", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    }else if (L->getType()==shortType) {
                        auto Str = helper->createCall("__spl__constructor__String__short", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    }else if (L->getType()==intType) {
                        auto Str = helper->createCall("__spl__constructor__String__int", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (L->getType()==longType) {
                        auto Str = helper->createCall("__spl__constructor__String__long", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    }else if (L->getType()==floatType) {
                        auto Str = helper->createCall("__spl__constructor__String__float", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else if (L->getType()==doubleType) {
                        auto Str = helper->createCall("__spl__constructor__String__double", {L});
                        destructAfterStatement.push_back(DestructAfterStatement(Str, "String", false));
                        auto tmp = helper->createCall("String.concat__spl__String__String__String", {Str, R});
                        destructAfterStatement.push_back(DestructAfterStatement(tmp, "String", true));
                        return tmp;
                    } else {
                        Out::errorMessage("Can not get primitive type");
                    }
                } else {
                    Out::errorMessage("Can not concat class to string");
                }
            }
        } else if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPAdd(helper->castToDouble(L),
                                       helper->castToDouble(R));
        } else if (helper->isIntValue(L) && helper->isIntValue(R)) {
            return helper->createAdd(L, R);
        } else {
            Out::errorMessage("Can not add values: incorrect types");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPSub(helper->castToDouble(L),
                                       helper->castToDouble(R));
        } else {
            return helper->createSub(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPMul(helper->castToDouble(L),
                                       helper->castToDouble(R));
        } else {
            return helper->createMul(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV) {
        if (helper->isFloatingPointValue(L) ||
            helper->isFloatingPointValue(R)) {
            return helper->createFPDiv(helper->castToDouble(L),
                                       helper->castToDouble(R));
        } else {
            return helper->createDiv(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD) {
    }
    Out::errorMessage("BUG! Can not return value for binary operation");
    return nullptr;
}

Value *CodeGen::genNewNode(shared_ptr<NewNode> node) {
    string str = "__spl__constructor__" + node->type->getFullName();
    for (auto arg : node->args) {
        str += "__" + arg->getReturnType()->getFullName();
    }
    vector<Value *> args{};
    for (auto arg : node->args) {
        args.push_back(genExpression(arg));
    }
    auto tmp = helper->createCall(str, args);
    destructAfterStatement.push_back(
        DestructAfterStatement(tmp, node->type->getFullName(), false));
    return tmp;
}

 CodeGen::DestructAfterStatement::DestructAfterStatement(Value *_val,
                                                        string _type,
                                                        bool _decreaseRefs) : val(_val), type(_type), decreaseRefs(_decreaseRefs) {}
