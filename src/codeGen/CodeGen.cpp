#include "CodeGen.hpp"
#include <utils/Out.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <main.hpp>

#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/PassBuilder.h>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

CodeGen::CodeGen(shared_ptr<CompilationUnitNode> _cu) : cu(_cu) {
    string moduleName = "__unnamedModule";
    if (cu->nodes[0]->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
        moduleName = "";
        for (string str : static_pointer_cast<PackageDeclNode>(cu->nodes[0])->name) {
            if (str != "") {
                moduleName += str + ".";
            }
        }
        moduleName.pop_back();
    }

    helper = make_shared<LLVMHelper>(moduleName);
    utils = make_shared<CodeGenUtils>(this);
}

void CodeGen::codeGen() {
    if (helper->getModuleName() == "__unnamedModule" ||
        helper->getModuleName() != "spl.core") {
        genImport(make_shared<ImportDeclNode>(vector<string>({"spl", "core"}), nullptr));
    }
    
    helper->createFunctionPrototype("__spl__init__gcmap", helper->getVoidType(), vector<Type*>{});
    helper->createFunctionPrototype("__spl__destroy__gcmap", helper->getVoidType(), vector<Type*>{});
    helper->createFunctionPrototype("__spl__alloc", helper->getPointerType(helper->getVoidType()), vector<Type*>{helper->getIntType(32)});
    helper->createFunctionPrototype("__spl__write", helper->getVoidType(), vector<Type*>{helper->getPointerType(helper->getVoidType()), helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype("__spl__destroyvar", helper->getVoidType(), vector<Type*>{helper->getPointerType(helper->getVoidType()), helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype("__spl__dec__refs", helper->getVoidType(), vector<Type*>{helper->getPointerType(helper->getVoidType())});
    helper->createFunctionPrototype("__spl__inc__refs", helper->getVoidType(), vector<Type*>{helper->getPointerType(helper->getVoidType())});

    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::IMPORT_DECL_NODE) {
            genImport(static_pointer_cast<ImportDeclNode>(node));
        } else if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            createClassType(static_pointer_cast<ClassDeclNode>(node));
        } else {
            Out::errorMessage("Can not generate this node");
        }
    }
    
    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            genStruct(static_pointer_cast<ClassDeclNode>(node));
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
}

void CodeGen::build() {
    helper->printModule();
    helper->prepareBuild();
    helper->runPasses();


    string Filename = helper->getModuleName() +".o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
    }

    helper->build(dest);
    Main::obj_files.push_back(Filename);
}

void CodeGen::genImport(shared_ptr<ImportDeclNode> node) {
    auto importFiles = Main::currCUsStack.top()->importFiles[node->name];
    if (node->name[0] == "spl" && node->name[1] == "core") {
        for (auto importFile : importFiles) {
            auto dir = importFile.getParent();
            if (dir.isDir()) {
                if (dir.getFilename() == "core") {
                    if (dir.getParent().isDir()) {
                        if (dir.getParent().getFilename() == "spl") {
                            auto ll_file = Path(dir.getName()+"/spl.core.stdlib.ll");
                            string o_file = dir.getName()+"/spl.core.stdlib.o";
                            Main::currCUsStack.top()->linkingObj.push_back(o_file);
                            system(string("clang "+ll_file.getName()+" -c -o "+o_file).c_str());
                            break;
                        }
                    }
                }
            }
        }
    }
    for (auto p : importFiles) {
        Main::CUs[p]->completeToState(CU::State::AST);
        auto importCU = Main::CUs[p]->cu;
        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
                continue;
            } else if (n->kind == Node::NodeKind::IMPORT_DECL_NODE) {
                genImport(static_pointer_cast<ImportDeclNode>(n));
            } else if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                createClassType(static_pointer_cast<ClassDeclNode>(n));
            } else {
                Out::errorMessage("Can not generate this node");
            }
        }

        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                genStruct(static_pointer_cast<ClassDeclNode>(n));
            }
        }

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

void CodeGen::createClassType(shared_ptr<ClassDeclNode> node) {
    utils->currClass = node;
    classesStack.push(node);
    utils->setCurrClassName();

    StructType *structType = helper->createStructType(utils->currClassName);
    utils->classesTypes.insert({utils->currClassName, structType});

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        createClassType(item);
    }

    genDestructorPrototype(node);

    for (shared_ptr<MethodDeclNode> item : node->methods) {
        genMethodPrototype(item);
    }

    for (shared_ptr<ConstructorDeclNode> item : node->constructors) {
        genConstructorPrototype(item);
    }
    if (node->constructors.empty()) {
        genConstructorPrototype(make_shared<ConstructorDeclNode>(nullptr, nullptr, vector<shared_ptr<VarDeclNode>>{}, nullptr, nullptr));
    }

    classesStack.pop();
    utils->currClass = classesStack.empty() ? nullptr : classesStack.top();
    utils->setCurrClassName();
}

void CodeGen::genClassDecl(shared_ptr<ClassDeclNode> node, bool genMethod) {
    utils->currClass = node;
    classesStack.push(node);
    utils->setCurrClassName();

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
            genConstructorDecl(make_shared<ConstructorDeclNode>(nullptr, nullptr, 
                                                                vector<shared_ptr<VarDeclNode>>{},
                                                                make_shared<BlockNode>(vector<shared_ptr<Node>>{}, nullptr),
                                                                nullptr));
        }
    }
    
    classesStack.pop();
    utils->currClass = classesStack.empty() ? nullptr : classesStack.top();
    utils->setCurrClassName();
}

void CodeGen::genStruct(shared_ptr<ClassDeclNode> node) {
    vector<Type*> types = vector<Type*>();

    for (shared_ptr<VarDeclNode> var : node->fields) {
        types.push_back(utils->getType(var->type->type->record));
        varTypes[var->record] = utils->getType(var->type->type->record);
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
            GlobalVariable *ptr = helper->createGlobalVar(helper->getPointerType(utils->getType(var->type->type->record)), var->getFullName());
            GlobalNamedValues[var->getFullName()] = ptr;
            ConstantPointerNull* const_ptr_2 = helper->getNullptr(helper->getPointerType(utils->getType(var->type->type->record)));
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
}

Function* CodeGen::genMethodPrototype(shared_ptr<MethodDeclNode> node) {
    vector<Type*> args_types = vector<Type*>();
    // TODO STATIC
    for (shared_ptr<VarDeclNode> arg : node->args) {
        args_types.push_back(utils->getType(arg->type->type->record));
    }
    return helper->createFunctionPrototype(node->getFullName(), utils->getType(node->returnType->type->record), args_types);
}

Function* CodeGen::genDestructorPrototype(shared_ptr<ClassDeclNode> node) {
    vector<Type*> args_types = vector<Type*>();
    args_types.push_back(utils->getType(utils->currClass->record));
    return helper->createFunctionPrototype("__spl__destructor__"+utils->currClassName, helper->getVoidType(), args_types);
}

Function* CodeGen::genConstructorPrototype(shared_ptr<ConstructorDeclNode> node) {
    string str = "__spl__constructor__"+utils->currClassName;
    for (auto arg : node->args) {
        str += "__" + arg->type->getFullName();
    }
    vector<Type*> args_types = vector<Type*>();
    for (shared_ptr<VarDeclNode> arg : node->args) {
        args_types.push_back(utils->getType(arg->type->type->record));
    }
    return helper->createFunctionPrototype(str, utils->getType(utils->currClass->record), args_types);
}

Function* CodeGen::genMethodDecl(shared_ptr<MethodDeclNode> node) {
    if (node->body != nullptr) {
        Function *TheFunction = helper->getFunction(node->getFullName());

        vector<Type*> args_types = vector<Type*>();
        bool isStatic = false;
        for (auto mod : node->modifiers->modifiers) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (!isStatic) {
            args_types.push_back(utils->getType(utils->currClass->record));
        }
        for (shared_ptr<VarDeclNode> arg : node->args) {
            args_types.push_back(utils->getType(arg->type->type->record));
        }

        if (!TheFunction) {
            return nullptr;
        }

        BasicBlock* BB = helper->createBBinFunc("entry", TheFunction);
        retBB = helper->createBBinFunc("ret", TheFunction);
        helper->activateBB(BB);

        NamedValues.clear();

        Value *ret_ptr;
        if (TheFunction->getReturnType() != helper->getVoidType()) {
            ret_ptr = helper->createAlloca(TheFunction->getReturnType(), nullptr, "retallocatmp");
            NamedValues[node->getFullName()+"__spl__ret"] = ret_ptr;
            helper->createStore(genDefaultValue(node->returnType), ret_ptr);
        }
        
        for (int i = 0; i < node->args.size(); ++i) {
            auto Arg = TheFunction->getArg(i);
            string argName = node->args[i]->getFullName();
            Value *ptr = helper->createAlloca(Arg->getType(), nullptr, argName);
            helper->createStore(Arg, ptr);
            NamedValues[argName] = ptr;
            varTypes[node->args[i]->record] = Arg->getType();
        }

        bool br = genBlockStatement(node->body);
        if (!br) {
            helper->createBr(retBB);
        }

        helper->activateBB(retBB);

        if (TheFunction->getReturnType() != helper->getVoidType()) {
            Value *ret_val = helper->createLoad(TheFunction->getReturnType(), ret_ptr, "retloadtmp");
            helper->createRet(ret_val);
        } else {
            helper->createRet(UndefValue::get(helper->getVoidType()));
        }

        verifyFunction(*TheFunction);

        if (node->returnType->type->record->id == "int") {
                if (node->record->id == "main") {
                    if (node->args.size() == 1) {
                        if (node->args[0]->type->type->record->id == "String") {
                            Function *MainFunction = helper->createFunctionPrototype("main", helper->getIntType(32), vector<Type*>());

                            if (!MainFunction) {
                                return nullptr;
                            }

                            BasicBlock* mainBB = helper->createBBinFunc("entry", MainFunction);
                            helper->activateBB(mainBB);

                            helper->createCall("__spl__init__gcmap");

                            for (auto globInit : StaticGlobalsInit) {
                                if (globInit.second != nullptr) {
                                    Value *ptr = globInit.first;
                                    Value *val = genExpression(globInit.second);
                                    helper->createStore(val, ptr);
                                }
                            }

                            Value *RetVal = helper->createCall(TheFunction, vector<Value*>({helper->getNullptr(static_cast<PointerType*>(args_types[0]))}));

                            helper->createCall("__spl__destroy__gcmap");

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
}

Function* CodeGen::genDestructorDecl(shared_ptr<ClassDeclNode> node) {
    
    Function *TheFunction = helper->getFunction("__spl__destructor__"+utils->currClassName);

    vector<Type*> args_types = vector<Type*>();
    args_types.push_back(utils->getType(utils->currClass->record));
    
    if (!TheFunction) {
        return nullptr;
    }

    BasicBlock* BB = helper->createBBinFunc("entry", TheFunction);
    retBB = helper->createBBinFunc("ret", TheFunction);
    helper->activateBB(BB);

    NamedValues.clear();

    Value *val = TheFunction->getArg(0);
    for (int i = 0; i < node->fields.size(); ++i) {
        Value* ptr = helper->createGetElementPtr(utils->getTypeNoPtr(node->record), val,
                                                 vector<Value*>{helper->getConstInt(32, 0), helper->getConstInt(32, i)});
        Value *loadptr = helper->createLoad(utils->getType(node->fields[i]->type->getReturnType()), ptr);
        if (node->fields[i]->type->type->record->type != "primitive") {
            helper->createCall("__spl__destroyvar",
                               vector<Value*>{loadptr, helper->getFunction("__spl__destructor__"+node->fields[i]->type->getFullName())});
        }
    }

    helper->createBr(retBB);
        
    helper->activateBB(retBB);

    helper->createRet(UndefValue::get(helper->getVoidType()));

    verifyFunction(*TheFunction);

    return TheFunction;
}

Function* CodeGen::genConstructorDecl(shared_ptr<ConstructorDeclNode> node) {
    string str = "__spl__constructor__"+utils->currClassName;
    for (auto arg : node->args) {
        str += "__" + arg->type->getFullName();
    }
    Function *TheFunction = helper->getFunction(str);

    vector<Type*> args_types = vector<Type*>();
    for (shared_ptr<VarDeclNode> arg : node->args) {
        args_types.push_back(utils->getType(arg->type->type->record));
    }
    if (!TheFunction) {
        return nullptr;
    }

    BasicBlock* BB = helper->createBBinFunc("entry", TheFunction);
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

bool CodeGen::genBlockStatement(shared_ptr<BlockNode> node) {
    bool ret = false;
    currBlockVars.push(vector<pair<Value*, string>>());
    for (shared_ptr<Node> item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == Node::NodeKind::BLOCK_NODE) {
                ret = genBlockStatement(static_pointer_cast<BlockNode>(item));
                break;
            } else if (item->kind == Node::NodeKind::RETURN_NODE) {
                if (static_pointer_cast<ReturnNode>(item)->expression != nullptr) {
                    Value *ptr = NamedValues[helper->getCurrFunction()->getName().str()+"__spl__ret"];
                    Value *val = genExpression(static_pointer_cast<ReturnNode>(item)->expression);
                    helper->createCall("__spl__write", vector<Value*>{ptr, val});
                    helper->createStore(val, ptr);
                }
                ret = true;
                break;
            } else if (item->kind == Node::NodeKind::VAR_DECL_NODE) {
                genVarDecl(static_pointer_cast<VarDeclNode>(item));
            } else if (item->kind == Node::NodeKind::VARS_DECL_NODE) {
                for (shared_ptr<VarDeclNode> decl : static_pointer_cast<VarsDeclNode>(item)->decls) {
                    genVarDecl(decl);
                }
            } else if (item->isExpression()) {
                genExpression(static_pointer_cast<ExpressionNode>(item));
            } else if (item->kind == Node::NodeKind::IF_ELSE_NODE) {
                genIfElse(static_pointer_cast<IfElseNode>(item));
            } else if (item->kind == Node::NodeKind::WHILE_NODE) {
                genWhile(static_pointer_cast<WhileNode>(item));
            } else if (item->kind == Node::NodeKind::FOR_NODE) {
                genFor(static_pointer_cast<ForNode>(item));
            }
        }
    }
    
    for (auto v : currBlockVars.top()) {
        Value *val = helper->createLoad(v.first->getType(), v.first);
        if (v.second != "boolean" && 
            v.second != "int" && 
            v.second != "byte" && 
            v.second != "short" && 
            v.second != "long" && 
            v.second != "float" && 
            v.second != "double" && 
            v.second != "char" && 
            v.second != "void") {

            helper->createCall("__spl__destroyvar", vector<Value*>{val, helper->getFunction("__spl__destructor__"+v.second)});
        }
    }
    if (ret) {
        helper->createBr(retBB);
    }
    currBlockVars.pop();
    return ret;
}

bool CodeGen::genConstructorBlockStatement(shared_ptr<ConstructorDeclNode> constructor) {
    shared_ptr<BlockNode> node = constructor->body;
    currBlockVars.push(vector<pair<Value*, string>>());
    
    Type *type = utils->getTypeNoPtr(utils->currClass->record);
    Value *sizeofIV = helper->createSizeof(type);

    Value *heapallocatmp = helper->createCall("__spl__alloc", vector<Value *>{sizeofIV}, "heapallocatmp");
    Value *ptr = helper->createAlloca(heapallocatmp->getType(), nullptr, string(heapallocatmp->getName())+"tmp_var");
    helper->createStore(heapallocatmp, ptr);
    
    for (int i = 0; i < utils->currClass->fields.size(); i++) {
        shared_ptr<VarRecord> n_var_rec = utils->currClass->fields[i]->record;
        auto classRecord = utils->currClass->record;
        Type *t = utils->getTypeNoPtr(classRecord);
        int struct_n = 0;
        for (int j = 0; j < classRecord->fields.size(); ++j) {
            if (classRecord->fields[j]->equals(n_var_rec)) {
                struct_n = j;
                break;
            }
        }
        Value *nullV = helper->getConstInt(32, 0);
        Value *struct_nV = helper->getConstInt(32, struct_n);
        Value *getelementptr = helper->createGetElementPtr(t, heapallocatmp, vector<Value*>{nullV, struct_nV}, "access_tmp");
        
        auto n_classRecord = utils->currClass->fields[i]->type->type->record;
        Type *n_t = utils->getTypeNoPtr(n_classRecord);
        auto last = helper->createLoad(n_t, getelementptr, "loadgetelementptrtmp");
        auto last_ptr = getelementptr;

        helper->createStore(genDefaultValue(make_shared<TypeNode>(make_shared<ClassRecordNode>(n_classRecord, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr)), last_ptr);
    }

    for (shared_ptr<Node> item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == Node::NodeKind::BLOCK_NODE) {
                return genBlockStatement(static_pointer_cast<BlockNode>(item));
            } else if (item->kind == Node::NodeKind::RETURN_NODE) {
                if (static_pointer_cast<ReturnNode>(item)->expression != nullptr) {
                    Value *val = genExpression(static_pointer_cast<ReturnNode>(item)->expression);
                    Value *ptr = NamedValues[helper->getCurrFunction()->getName().str()+"__spl__ret"];
                    helper->createStore(val, ptr);
                }
                helper->createBr(retBB);
                return true;
            } else if (item->kind == Node::NodeKind::VAR_DECL_NODE) {
                genVarDecl(static_pointer_cast<VarDeclNode>(item));
            } else if (item->kind == Node::NodeKind::VARS_DECL_NODE) {
                for (shared_ptr<VarDeclNode> decl : static_pointer_cast<VarsDeclNode>(item)->decls) {
                    genVarDecl(decl);
                }
            } else if (item->isExpression()) {
                genExpression(static_pointer_cast<ExpressionNode>(item));
            } else if (item->kind == Node::NodeKind::IF_ELSE_NODE) {
                genIfElse(static_pointer_cast<IfElseNode>(item));
            } else if (item->kind == Node::NodeKind::WHILE_NODE) {
                genWhile(static_pointer_cast<WhileNode>(item));
            } else if (item->kind == Node::NodeKind::FOR_NODE) {
                genFor(static_pointer_cast<ForNode>(item));
            }
        }
    }
    for (auto v : currBlockVars.top()) {
        Value *val = helper->createLoad(v.first->getType(), v.first);
        if (v.second != "boolean" && 
            v.second != "int" && 
            v.second != "byte" && 
            v.second != "short" && 
            v.second != "long" && 
            v.second != "float" && 
            v.second != "double" && 
            v.second != "char" && 
            v.second != "void") {

            helper->createCall("__spl__destroyvar", vector<Value*>{val, helper->getFunction("__spl__destructor__"+v.second)});
        }
    }
    currBlockVars.pop();

    helper->createRet(heapallocatmp);
}

void CodeGen::genIfElse(shared_ptr<IfElseNode> node) {
    Value *cond = genExpression(node->condition);

    Function *TheFunction = helper->getCurrFunction();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
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
    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->thenNode}, nullptr));
    if (!br) {
        helper->createBr(MergeBB);
    }
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = helper->getActiveBB();

    if (node->elseNode != nullptr) {
        // Emit else block.
        helper->activateBB(ElseBB);
        br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->elseNode}, nullptr));
        if (!br) {
            helper->createBr(MergeBB);
        }
        // codegen of 'Else' can change the current block, update ElseBB for the PHI.
        ElseBB = helper->getActiveBB();
    }
    

    helper->activateBB(MergeBB);
}

void CodeGen::genWhile(shared_ptr<WhileNode> node) {
    Value *cond = genExpression(node->expression);

    Function *TheFunction = helper->getCurrFunction();

    BasicBlock *whileBB = helper->createBBinFunc("while", TheFunction);
    BasicBlock *whilebodyBB = helper->createBBinFunc("whilebody", TheFunction);
    BasicBlock *whilecontBB = helper->createBBinFunc("whilecont", TheFunction);

    helper->createBr(whileBB);

    helper->activateBB(whileBB);

    helper->createIfElse(cond, whilebodyBB, whilecontBB);

    helper->activateBB(whilebodyBB);

    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->statement}, nullptr));
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

    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->init}, nullptr));
    if (br) {
        return;
    } 
    helper->createBr(forcondBB);

    helper->activateBB(forcondBB);

    Value *cond = genExpression(node->condition);
    helper->createIfElse(cond, forbodyBB, forcontBB);

    helper->activateBB(forbodyBB);

    br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->statement}, nullptr));
    if (!br) {
        br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->update}, nullptr));
        if (br) {
            return;
        } 
        helper->createBr(forcondBB);
    }
    // TODO br forcont
    helper->activateBB(forcontBB);
}

Value* CodeGen::genExpression(shared_ptr<ExpressionNode> node) {
    if (node->isLiteral()) {
        return genLiteral(node);
    } else if (node->kind == Node::NodeKind::METHOD_CALL_NODE) {
        shared_ptr<MethodCallNode> callNode = static_pointer_cast<MethodCallNode>(node);
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
        return genBinOp(static_pointer_cast<BinaryOperatorNode>(node));
    } else if (node->kind == Node::NodeKind::VAR_RECORD_NODE) {
        return genVarValue(static_pointer_cast<VarRecordNode>(node));
    } else if (node->kind == Node::NodeKind::NEW_NODE) {
        return genNewNode(static_pointer_cast<NewNode>(node));
    } else if (node->kind == Node::NodeKind::ARRAY_CREATION_NODE) {
        auto n = static_pointer_cast<ArrayCreationNode>(node);
        if (n->isStatic) {
            Type* lastTy = utils->getType(n->type->type->record);
            Value* mul = helper->getConstInt(64, 1);
            for (int i = n->dims.size()-1; i >= 0; --i) {
                if (n->dims[i]->kind == Node::NodeKind::INT_LITERAL_NODE) {
                    lastTy = helper->getArrayType(lastTy, static_pointer_cast<IntLiteralNode>(n->dims[i])->value);
                } else {
                    Value* expr = genExpression(n->dims[i]);
                    if (expr->getType()->isIntegerTy()) {
                        if (expr->getType()->getPrimitiveSizeInBits() < 64) {
                            expr = CastInst::CreateZExtOrBitCast(expr, helper->getIntType(64));
                        }
                    } else {
                        Out::errorMessage("Array size is not int type!");
                    }
                    mul = helper->createMul(mul, expr);
                }
            }
            Value* alloca = helper->createAlloca(lastTy, mul);
            return alloca;
        } else {
            Out::errorMessage("No dynamic arrays support yet.");
        }
    } else if (node->kind == Node::NodeKind::ARRAY_INITIALIZER_NODE) {

    } else if (node->kind == Node::NodeKind::ARRAY_ACCESS_NODE) {
        auto n = static_pointer_cast<ArrayAccessNode>(node);
        Value* arr = genExpression(n->array);
        // TODO
    } else if (node->kind == Node::NodeKind::ACCESS_NODE) {
        shared_ptr<AccessNode> access = static_pointer_cast<AccessNode>(node);
        Value *val = genExpression(static_pointer_cast<ExpressionNode>(access->access[0]));
        shared_ptr<ClassRecord> val_type = nullptr;
        if (access->access[0]->kind == Node::NodeKind::VAR_RECORD_NODE) {
            val_type = static_pointer_cast<VarRecordNode>(access->access[0])->getReturnType();
        } else if (access->access[0]->kind == Node::NodeKind::METHOD_CALL_NODE) {
            val_type = static_pointer_cast<MethodCallNode>(access->access[0])->getReturnType();
        } else if (access->access[0]->kind == Node::NodeKind::ARRAY_ACCESS_NODE) {
            // TODO
            //val_type = static_pointer_cast<MethodCallNode>(access->access[0])->getReturnType();
        } else {
            Out::errorMessage("Can not generate expression");
        }
        for (int i = 1; i < access->access.size(); ++i) {
            if (access->access[i]->kind == Node::NodeKind::VAR_RECORD_NODE) {
                int n = -1;
                for (int j = 0; j < val_type->fields.size(); ++j) {
                    if (val_type->fields[j]->id == static_pointer_cast<VarRecordNode>(access->access[i])->record->id) {
                        n = j;
                        break;
                    }
                }
                if (n == -1) {
                    Out::errorMessage("Can not find field in class");
                }
                vector<Value*> ids = vector<Value*>{helper->getConstInt(32, 0), helper->getConstInt(32, n)};
                Value *getelementptr = helper->createGetElementPtr(utils->getTypeNoPtr(val_type), val, ids);
                val_type = static_pointer_cast<VarRecordNode>(access->access[i])->getReturnType();
                val = helper->createLoad(utils->getType(val_type), getelementptr);
            } else if (access->access[i]->kind == Node::NodeKind::METHOD_CALL_NODE) {
                // TODO
            }
        }
        return val;
    }
    return nullptr;
}

Value* CodeGen::genLiteral(shared_ptr<ExpressionNode> node) {
    if (node->kind == Node::NodeKind::INT_LITERAL_NODE) {
        if (static_pointer_cast<IntLiteralNode>(node)->longVal) {
            return helper->getConstInt(64, static_pointer_cast<IntLiteralNode>(node)->value);
        } else {
            return helper->getConstInt(32, static_pointer_cast<IntLiteralNode>(node)->value);
        }
    } else if (node->kind == Node::NodeKind::BOOL_LITERAL_NODE) {
        return helper->getConstInt(1, static_pointer_cast<BoolLiteralNode>(node)->value);
    } else if (node->kind == Node::NodeKind::CHAR_LITERAL_NODE) {
        return helper->getConstInt(8, static_pointer_cast<CharLiteralNode>(node)->str[0]);
    } else if (node->kind == Node::NodeKind::FLOAT_LITERAL_NODE) {
        if (static_pointer_cast<FloatLiteralNode>(node)->doubleVal) {
            return helper->getConstDouble(static_pointer_cast<FloatLiteralNode>(node)->value);
        } else {
            return helper->getConstFloat(static_pointer_cast<FloatLiteralNode>(node)->value);
        }
    }
}

Value* CodeGen::genMethodCall(shared_ptr<MethodCallNode> node, Value *calle) {
    if (node->record->similar.empty()) {
        vector<Value*> args = vector<Value*>();
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
        if (tmp->getType() != helper->getVoidType()) {
            helper->createCall("__spl__dec__refs", vector<Value*>{tmp});
        }
        return tmp;
    } else {
        vector<Value*> args = vector<Value*>();
        if (calle != nullptr) {
            args.push_back(calle);
            thisV.push(calle);
        }
        for (shared_ptr<ExpressionNode> arg : node->args) {
            args.push_back(genExpression(arg));
        }

        auto funcsRecord = node->record->similar;
        for (auto funRecord : funcsRecord) {
            Function *TheFunction = helper->getFunction(funRecord->getFullName());
            bool same = true;
            for (int i = 0; i < args.size(); ++i) {
                if (args[i]->getType() != TheFunction->getArg(i)->getType()) {
                    same = false;
                }
            }
            if (same) {
                auto tmp = helper->createCall(TheFunction, args);
                if (calle != nullptr) {
                    thisV.pop();
                }
                if (tmp->getType() != helper->getVoidType()) {
                    helper->createCall("__spl__dec__refs", vector<Value*>{tmp});
                }
                return tmp;
            }
        }
        auto funRecord = node->record;
        string fullName = funRecord->getFullName();
        Function *TheFunction = helper->getFunction(fullName);
        bool same = true;
        for (int i = 0; i < args.size(); ++i) {
            if (args[i]->getType() != TheFunction->getArg(i)->getType()) {
                same = false;
            }
        }
        if (same) {
            auto tmp = helper->createCall(TheFunction, args);
            if (calle != nullptr) {
                thisV.pop();
            }
            if (tmp->getType() != helper->getVoidType()) {
                helper->createCall("__spl__dec__refs", vector<Value*>{tmp});
            }
            return tmp;
        }

        Out::errorMessage("Fatal error! Can not car record of method call. Undefined reference: " + node->record->id);
    }
    
}

Value* CodeGen::genVarDecl(shared_ptr<VarDeclNode> node) {
    Value *val = node->init != nullptr ? genExpression(node->init) : genDefaultValue(node->type);
    Value *ptr = helper->createAlloca(val->getType(), nullptr, node->getFullName());
    if (val->getType()->isPointerTy()) {
        helper->createCall("__spl__write", vector<Value *>{ptr, val});
    }
    helper->createStore(val, ptr);
    NamedValues[node->getFullName()] = ptr;
    varTypes[node->record] = val->getType();
    currBlockVars.top().push_back(pair<Value*, string>(ptr, node->type->getFullName()));
}

Value* CodeGen::genDefaultValue(shared_ptr<TypeNode> node) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "boolean") {
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
}

Value* CodeGen::genVarValue(shared_ptr<VarRecordNode> node) {
    Value *ptr = NamedValues[node->getFullName()];
    if (ptr == nullptr) {
        ptr = GlobalNamedValues[node->getFullName()];
    }
    Type *type = varTypes[node->record];
    Value *val = helper->createLoad(type, ptr);
    return val; 
}

Value* CodeGen::genBinOp(shared_ptr<BinaryOperatorNode> node) {
    auto L = genExpression(node->left);
    auto R = genExpression(node->right);

    if (node->op == BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_OR_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_AND_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD_ASSIGN) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ASSIGN) {
        if (node->left->kind == Node::NodeKind::ACCESS_NODE) {
            shared_ptr<AccessNode> access = static_pointer_cast<AccessNode>(node->left);
            Value *ptr = nullptr;
            Value *val = nullptr;
            shared_ptr<ClassRecord> val_type = nullptr;
            if (access->access[0]->kind == Node::NodeKind::VAR_RECORD_NODE) {
                shared_ptr<VarRecordNode> var = static_pointer_cast<VarRecordNode>(access->access[0]);
                ptr = NamedValues[var->getFullName()];
                if (ptr == nullptr) {
                    ptr = GlobalNamedValues[var->getFullName()];
                }
                Type *type = varTypes[var->record];
                val = helper->createLoad(type, ptr);
                val_type = var->getReturnType();
            } else if (access->access[0]->kind == Node::NodeKind::METHOD_CALL_NODE) {
                Out::errorMessage("Can not assign to method call result");
            } else {
                Out::errorMessage("Can not generate expression");
            }
            for (int i = 1; i < access->access.size(); ++i) {
                if (access->access[i]->kind == Node::NodeKind::VAR_RECORD_NODE) {
                    int n = -1;
                    for (int j = 0; j < val_type->fields.size(); ++j) {
                        if (val_type->fields[j]->id == static_pointer_cast<VarRecordNode>(access->access[i])->record->id) {
                            n = j;
                            break;
                        }
                    }
                    if (n == -1) {
                        Out::errorMessage("Can not find field in class");
                    }
                    vector<Value*> ids = vector<Value*>{helper->getConstInt(32, 0), helper->getConstInt(32, n)};
                    Value *getelementptr = helper->createGetElementPtr(utils->getTypeNoPtr(val_type), val, ids);
                    val_type = static_pointer_cast<VarRecordNode>(access->access[i])->getReturnType();
                    val = helper->createLoad(utils->getType(val_type), getelementptr);
                    ptr = getelementptr;
                } else if (access->access[i]->kind == Node::NodeKind::METHOD_CALL_NODE) {
                    // TODO
                }
            }
            helper->createCall("__spl__write", vector<Value*>{ptr, R});
            helper->createStore(R, ptr);

        } else {
            Out::errorMessage("Can not create assign.");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::OR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::AND) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_OR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_AND) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::EQUAL) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPEQ(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createEQ(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPNE(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createNE(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPLT(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createLT(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPGT(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createGT(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPLE(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createLE(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPGE(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createGE(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPAdd(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createAdd(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPSub(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createSub(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPMul(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createMul(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV) {
        if (helper->isFloatingPointValue(L) || helper->isFloatingPointValue(R)) {
            return helper->createFPDiv(helper->castToDouble(L), helper->castToDouble(R));
        } else {
            return helper->createDiv(L, R);
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD) {
        
    }
}

Value* CodeGen::genNewNode(shared_ptr<NewNode> node) {
    string str = "__spl__constructor__"+node->type->getFullName();
    for (auto arg : node->args) {
        str += "__" + arg->getReturnType()->getFullName();
    }
    vector<Value*> args{};
    for (auto arg : node->args) {
        args.push_back(genExpression(arg));
    }
    auto tmp =  helper->createCall(str, args);
    Value *ptr = helper->createAlloca(tmp->getType(), nullptr, string(tmp->getName())+"tmp_var");
    helper->createStore(tmp, ptr);
    currBlockVars.top().push_back(pair<Value*, string>(ptr, node->type->getFullName()));

    return tmp;
}
