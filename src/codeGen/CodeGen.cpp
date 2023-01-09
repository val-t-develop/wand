#include "CodeGen.hpp"
#include <utils/Out.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/NullNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <main.hpp>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

CodeGen::CodeGen(shared_ptr<CompilationUnitNode> _cu) : cu(_cu) {
    
    TheContext = make_shared<LLVMContext>();

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

    TheModule = make_shared<Module>(moduleName, *TheContext);
    Builder = make_shared<IRBuilder<>>(*TheContext);
}

void CodeGen::codeGen() {
    if (TheModule->getName() == "__unnamedModule" || 
        TheModule->getName() != "spl.core") {
        genImport(make_shared<ImportDeclNode>(vector<string>({"spl", "core"}), nullptr));
    }

    Function *splMallocFunction = TheModule->getFunction("__spl__malloc");
    FunctionType* splMallocFunction_ft = FunctionType::get(PointerType::get(*TheContext, 0), vector<Type*>{IntegerType::get(*TheContext, 32)}, false);
    splMallocFunction = Function::Create(splMallocFunction_ft, Function::ExternalLinkage, "__spl__malloc", *TheModule);

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
    TheModule->print(errs(), nullptr);
}

void CodeGen::build() {
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

    string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        errs() << Error;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    string Filename = string(TheModule->getName()) +".o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
    }

    legacy::PassManager pass;
    auto FileType = CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
    }

    pass.run(*TheModule);
    dest.flush();
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

Type* CodeGen::getType(shared_ptr<TypeNode> node, bool ptr = true) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "boolean") {
            return IntegerType::get(*TheContext, 1);
        } else if (x->record->id == "int") {
            return IntegerType::get(*TheContext, 32);
        } else if (x->record->id == "byte") {
            return IntegerType::get(*TheContext, 8);
        } else if (x->record->id == "short") {
            return IntegerType::get(*TheContext, 16);
        } else if (x->record->id == "long") {
            return IntegerType::get(*TheContext, 64);
        } else if (x->record->id == "float") {
            return Type::getFloatTy(*TheContext);
        } else if (x->record->id == "double") {
            return Type::getDoubleTy(*TheContext);
        } else if (x->record->id == "char") {
            return IntegerType::get(*TheContext, 8);
        } else if (x->record->id == "void") {
            return Type::getVoidTy(*TheContext);
        }
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(getFullClassRecordName(x->record))) {
            structType = classesTypes.at(getFullClassRecordName(x->record));
        } else {
            Out::errorMessage("Can not get " + getFullClassRecordName(x->record));
        }
        if (ptr) {
            PointerType *structPtrType = PointerType::get(structType, 0);
            return structPtrType;
        } else {
            return structType;
        }
    }
}

void CodeGen::createClassType(shared_ptr<ClassDeclNode> node) {
    currClass = node;
    classesStack.push(node);
    setCurrClassName();

    StructType *structType = StructType::create(*TheContext, currClassName);
    classesTypes.insert({currClassName, structType});

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        createClassType(item);
    }

    for (shared_ptr<MethodDeclNode> item : node->methods) {
        genMethodPrototype(item);
    }

    classesStack.pop();
    currClass = classesStack.empty() ? nullptr : classesStack.top();
    setCurrClassName();
}

void CodeGen::genClassDecl(shared_ptr<ClassDeclNode> node, bool genMethod) {
    currClass = node;
    classesStack.push(node);
    setCurrClassName();

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        genClassDecl(item, genMethod);
    }

    if (genMethod) {
        for (shared_ptr<MethodDeclNode> item : node->methods) {
            genMethodDecl(item);
        }
    }
    
    classesStack.pop();
    currClass = classesStack.empty() ? nullptr : classesStack.top();
    setCurrClassName();
}

void CodeGen::setCurrClassName() {
    if (currClass != nullptr) {
        string str = getFullClassRecordName(currClass->record);
        currClassName = str;
    } else {
        currClassName = "";
    }
}

string CodeGen::getFullClassRecordName(shared_ptr<ClassRecord> rec) {
    if (rec->ir_name == "") {
        string str = "";
        if (rec->next != nullptr) {
            str += getFullClassRecordName(static_pointer_cast<ClassRecord>(rec->next)) + ".";
        }
        str += rec->id;
        rec->ir_name = str;
    }
    return rec->ir_name;
}

string CodeGen::getFullMethodDeclNodeName(shared_ptr<MethodDeclNode> node) {
    shared_ptr<MethodRecord> rec = node->record;
    if (rec->ir_name == "") {
        string str = "";
        if (rec->next != nullptr) {
            str += getFullClassRecordName(static_pointer_cast<ClassRecord>(rec->next)) + ".";
        }
        str += rec->id;

        str += "__spl__" + getFullClassRecordName(node->returnType->type->record);
        for (auto arg : node->args) {
            str += "__" + getFullClassRecordName(arg->type->type->record);
        }
        rec->ir_name = str; 
    }
    return rec->ir_name;
}

string CodeGen::getFullVarDeclNodeName(shared_ptr<VarDeclNode> node) {
    shared_ptr<VarRecord> rec = node->record;
    if (rec->ir_name == "") {
        string str = "";
        if (rec->next != nullptr) {
            str += getFullClassRecordName(static_pointer_cast<ClassRecord>(rec->next)) + ".";
        }
        str += rec->id;

        str += "__spl__" + getFullClassRecordName(node->type->type->record);
        rec->ir_name = str; 
    }
    return rec->ir_name;
}

string CodeGen::getFullMethodRecordName(shared_ptr<MethodRecord> rec) {
    if (rec->ir_name == "") {
        Out::errorMessage("Internal error detected! Can not get ir_name of method " + rec->id);
    }
    return rec->ir_name;
}

string CodeGen::getFullVarRecordName(shared_ptr<VarRecord> rec) {
    if (rec->ir_name == "") {
        Out::errorMessage("Internal error detected! Can not get ir_name of var or field " + rec->id);
    }
    return rec->ir_name;
}

void CodeGen::genStruct(shared_ptr<ClassDeclNode> node) {
    vector<Type*> types = vector<Type*>();

    for (shared_ptr<VarDeclNode> var : node->fields) {
        types.push_back(getType(var->type));
        varTypes[var->record] = getType(var->type);
        getFullVarDeclNodeName(var);

        bool isStatic = false;
        auto mods = var->modifiers->modifiers;
        for (auto mod : mods) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }

        if (isStatic) {
            GlobalVariable *ptr = new GlobalVariable(*TheModule, PointerType::get(getType(var->type), 0), false, 
                                                        GlobalValue::LinkageTypes::ExternalLinkage, 0, getFullVarDeclNodeName(var));
            GlobalNamedValues[getFullVarDeclNodeName(var)] = ptr;
            ConstantPointerNull* const_ptr_2 = ConstantPointerNull::get(PointerType::get(getType(var->type), 0));
            ptr->setInitializer(const_ptr_2);
            StaticGlobalsInit[ptr] = var->init;
        }
        
    }
    string fullName = getFullClassRecordName(node->record);

    StructType *structType;
    if (classesTypes.contains(fullName)) {
        structType = classesTypes.at(fullName);
    } else {
        structType = nullptr;
        Out::errorMessage("Can not get " + fullName);
    }
    structType->setBody(types);
}

Function* CodeGen::genMethodPrototype(shared_ptr<MethodDeclNode> node) {
    Function *TheFunction = TheModule->getFunction(getFullMethodDeclNodeName(node));

    vector<Type*> args_types = vector<Type*>();
    if (!TheFunction) {
        
        for (shared_ptr<VarDeclNode> arg : node->args) {
            args_types.push_back(getType(arg->type));
        }
        FunctionType* ft = FunctionType::get(getType(node->returnType), args_types, false);

        TheFunction = Function::Create(ft, Function::ExternalLinkage, getFullMethodDeclNodeName(node), *TheModule);
    }

    if (!TheFunction) {
        return nullptr;
    }
    return TheFunction;
}

Function* CodeGen::genMethodDecl(shared_ptr<MethodDeclNode> node) {
    if (node->body != nullptr) {
        Function *TheFunction = TheModule->getFunction(getFullMethodDeclNodeName(node));

        vector<Type*> args_types = vector<Type*>();
        bool isStatic = false;
        for (auto mod : node->modifiers->modifiers) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (!isStatic) {
            args_types.push_back(getType(make_shared<TypeNode>(make_shared<ClassRecordNode>(currClass->record, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr)));
        }
        for (shared_ptr<VarDeclNode> arg : node->args) {
            args_types.push_back(getType(arg->type));
        }

        if (!TheFunction) {
            return nullptr;
        }
        
        BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
        retBB = BasicBlock::Create(*TheContext, "ret");
        Builder->SetInsertPoint(BB);

        NamedValues.clear();

        Value *ret_ptr;
        if (TheFunction->getReturnType() != Type::getVoidTy(*TheContext)) {
            ret_ptr = Builder->CreateAlloca(TheFunction->getReturnType(), nullptr, "retallocatmp");
            NamedValues[getFullMethodDeclNodeName(node)+"__spl__ret"] = ret_ptr;
        }
        
        for (int i = 0; i < node->args.size(); ++i) {
            auto Arg = TheFunction->getArg(i);
            string argName = getFullVarDeclNodeName(node->args[i]);
            Value *ptr = Builder->CreateAlloca(Arg->getType(), nullptr, argName);
            Builder->CreateStore(Arg, ptr);
            NamedValues[argName] = ptr;
            varTypes[node->args[i]->record] = Arg->getType();
        }

        bool br = genBlockStatement(node->body);
        if (!br) {
            Builder->CreateBr(retBB);
        }
        
        retBB->insertInto(TheFunction);
        Builder->SetInsertPoint(retBB);

        if (TheFunction->getReturnType() != Type::getVoidTy(*TheContext)) {
            Value *ret_val = Builder->CreateLoad(TheFunction->getReturnType(), ret_ptr, "retloadtmp");
            Builder->CreateRet(ret_val);
        } else {
            Value *ret_val = ReturnInst::Create(*TheContext);
            Builder->CreateRet(ret_val);
        }

        verifyFunction(*TheFunction);

        if (node->returnType->type->record->id == "int") {
                if (node->record->id == "main") {
                    if (node->args.size() == 1) {
                        if (node->args[0]->type->type->record->id == "String") {
                            Function *MainFunction = TheModule->getFunction("main");

                            if (!MainFunction) {
                                vector<Type*> args_types = vector<Type*>();
                                FunctionType* ft = FunctionType::get(IntegerType::get(*TheContext, 32), args_types, false);

                                MainFunction = Function::Create(ft, Function::ExternalLinkage, "main", *TheModule);
                            }

                            if (!MainFunction) {
                                return nullptr;
                            }
                            
                            BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", MainFunction);
                            Builder->SetInsertPoint(BB);

                            for (auto globInit : StaticGlobalsInit) {
                                if (globInit.second != nullptr) {
                                    Value *ptr = globInit.first;
                                    Value *val = genExpression(globInit.second);
                                    Builder->CreateStore(val, ptr);
                                }
                            }

                            Value *RetVal = Builder->CreateCall(TheFunction, vector<Value*>({ConstantPointerNull::get(static_cast<PointerType*>(args_types[0]))}), "calltmp");
                            if (RetVal) {
                                Builder->CreateRet(RetVal);
                                verifyFunction(*MainFunction);
                            }
                        }
                    }
                }
            }

        return TheFunction;
    }
}

bool CodeGen::genBlockStatement(shared_ptr<BlockNode> node) {
    for (shared_ptr<Node> item : node->nodes) {
        if (item != nullptr) {
            if (item->kind == Node::NodeKind::BLOCK_NODE) {
                return genBlockStatement(static_pointer_cast<BlockNode>(item));
            } else if (item->kind == Node::NodeKind::RETURN_NODE) {
                if (static_pointer_cast<ReturnNode>(item)->expression != nullptr) {
                    Value *val = genExpression(static_pointer_cast<ReturnNode>(item)->expression);
                    Value *ptr = NamedValues[Builder->GetInsertBlock()->getParent()->getName().str()+"__spl__ret"];
                    Builder->CreateStore(val, ptr);
                }
                Builder->CreateBr(retBB);
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
    return false;
}

void CodeGen::genIfElse(shared_ptr<IfElseNode> node) {
    Value *cond = genExpression(node->condition);

    Function *TheFunction = Builder->GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    BasicBlock *ThenBB =
        BasicBlock::Create(*TheContext, "then", TheFunction);
    BasicBlock *ElseBB = nullptr;
    if (node->elseNode != nullptr) {
        ElseBB = BasicBlock::Create(*TheContext, "else");
    }
    BasicBlock *MergeBB = BasicBlock::Create(*TheContext, "ifcont");

    if (node->elseNode != nullptr) {
        Builder->CreateCondBr(cond, ThenBB, ElseBB);
    } else {
        Builder->CreateCondBr(cond, ThenBB, MergeBB);
    }
    

    // Emit then value.
    Builder->SetInsertPoint(ThenBB);
    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->thenNode}, nullptr));
    if (!br) {
        Builder->CreateBr(MergeBB);
    }
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder->GetInsertBlock();

    if (node->elseNode != nullptr) {
        // Emit else block.
        ElseBB->insertInto(TheFunction);
        Builder->SetInsertPoint(ElseBB);
        br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->elseNode}, nullptr));
        if (!br) {
            Builder->CreateBr(MergeBB);
        }
        // codegen of 'Else' can change the current block, update ElseBB for the PHI.
        ElseBB = Builder->GetInsertBlock();
    }
    

    MergeBB->insertInto(TheFunction);
    Builder->SetInsertPoint(MergeBB);
}

void CodeGen::genWhile(shared_ptr<WhileNode> node) {
    Value *cond = genExpression(node->expression);

    Function *TheFunction = Builder->GetInsertBlock()->getParent();

    BasicBlock *whileBB = BasicBlock::Create(*TheContext, "while");
    BasicBlock *whilebodyBB = BasicBlock::Create(*TheContext, "whilebody");
    BasicBlock *whilecontBB = BasicBlock::Create(*TheContext, "whilecont");

    Builder->CreateBr(whileBB);

    whileBB->insertInto(TheFunction);
    Builder->SetInsertPoint(whileBB);

    Builder->CreateCondBr(cond, whilebodyBB, whilecontBB);

    whilebodyBB->insertInto(TheFunction);
    Builder->SetInsertPoint(whilebodyBB);

    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->statement}, nullptr));
    if (!br) {
        Builder->CreateBr(whileBB);
    }

    whilecontBB->insertInto(TheFunction);
    Builder->SetInsertPoint(whilecontBB);
}

void CodeGen::genFor(shared_ptr<ForNode> node) {

    Function *TheFunction = Builder->GetInsertBlock()->getParent();

    BasicBlock *forBB = BasicBlock::Create(*TheContext, "for");
    BasicBlock *forcondBB = BasicBlock::Create(*TheContext, "forcond");
    BasicBlock *forbodyBB = BasicBlock::Create(*TheContext, "forbody");
    BasicBlock *forcontBB = BasicBlock::Create(*TheContext, "forcont");

    Builder->CreateBr(forBB);

    forBB->insertInto(TheFunction);
    Builder->SetInsertPoint(forBB);

    bool br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->init}, nullptr));
    if (br) {
        return;
    } 
    Builder->CreateBr(forcondBB);

    forcondBB->insertInto(TheFunction);
    Builder->SetInsertPoint(forcondBB);

    Value *cond = genExpression(node->condition);
    Builder->CreateCondBr(cond, forbodyBB, forcontBB);

    forbodyBB->insertInto(TheFunction);
    Builder->SetInsertPoint(forbodyBB);

    br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->statement}, nullptr));
    if (!br) {
        br = genBlockStatement(make_shared<BlockNode>(vector<shared_ptr<Node>>{node->update}, nullptr));
        if (br) {
            return;
        } 
        Builder->CreateBr(forcondBB);
    }

    forcontBB->insertInto(TheFunction);
    Builder->SetInsertPoint(forcontBB);
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
    } else if (node->kind == Node::NodeKind::ACCESS_NODE) {
        shared_ptr<AccessNode> access = static_pointer_cast<AccessNode>(node);
        Value *last = nullptr;
        if (access->isExpression()) {
            for (int i = 0; i < access->access.size(); ++i) {
                auto n = access->access[i];
                if (i == 0) {
                    if (n->isExpression()) {
                        last = genExpression(static_pointer_cast<ExpressionNode>(n));
                        continue;
                    } else {
                        Out::errorMessage("Internall error detected: can not generate expression.");
                        break;
                    }
                }
                
                if (n->isExpression()) {
                    auto last_n = access->access[i-1];
                    if (n->kind == Node::NodeKind::VAR_RECORD_NODE) {
                        shared_ptr<VarRecord> n_var_rec = static_pointer_cast<VarRecordNode>(n)->record;
                        if (last_n->isExpression()) {
                            auto classRecord = static_pointer_cast<ExpressionNode>(last_n)->getReturnType();
                            shared_ptr<TypeNode> typeNode = make_shared<TypeNode>(make_shared<ClassRecordNode>(classRecord, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr);
                            Type *t = getType(typeNode, false);
                            int struct_n = 0;
                            for (int j = 0; j < classRecord->fields.size(); ++j) {
                                if (classRecord->fields[j]->equals(n_var_rec)) {
                                    struct_n = j;
                                    break;
                                }
                            }
                            Value *nullV = ConstantInt::getSigned(IntegerType::get(*TheContext, 32), 0);
                            Value *struct_nV = ConstantInt::getSigned(IntegerType::get(*TheContext, 32), struct_n);
                            Value *getelementptr = GetElementPtrInst::Create(t, last, vector<Value*>{nullV, struct_nV}, "access_tmp", Builder->GetInsertBlock());
                            
                            auto n_classRecord = static_pointer_cast<ExpressionNode>(n)->getReturnType();
                            shared_ptr<TypeNode> n_typeNode = make_shared<TypeNode>(make_shared<ClassRecordNode>(n_classRecord, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr);
                            Type *n_t = getType(n_typeNode, false);
                            last = Builder->CreateLoad(n_t, getelementptr, "loadgetelementptrtmp");
                        } else {
                            Out::errorMessage("Internall error detected: can not generate expression.");
                            break;
                        }
                    } else if (n->kind == Node::NodeKind::METHOD_CALL_NODE) {
                        shared_ptr<MethodCallNode> callNode = static_pointer_cast<MethodCallNode>(n);
                        bool isStatic = false;
                        for (auto mod : callNode->record->mods) {
                            if (mod == ModifiersNode::ModifierKind::STATIC) {
                                isStatic = true;
                                break;
                            }
                        }
                        if (isStatic) {
                            Out::errorMessage("Error: call of static method with non class access.");
                        } else {
                            last = genMethodCall(callNode, last);
                        }
                    }
                    continue;
                } else {
                    Out::errorMessage("Internall error detected: can not generate expression.");
                    break;
                }
            }
            return last;
        }
    }
    return nullptr;
}

Value* CodeGen::genLiteral(shared_ptr<ExpressionNode> node) {
    if (node->kind == Node::NodeKind::INT_LITERAL_NODE) {
        if (static_pointer_cast<IntLiteralNode>(node)->longVal) {
            return ConstantInt::getSigned(IntegerType::get(*TheContext, 64), static_pointer_cast<IntLiteralNode>(node)->value);
        } else {
            return ConstantInt::getSigned(IntegerType::get(*TheContext, 32), static_pointer_cast<IntLiteralNode>(node)->value);
        }
    } else if (node->kind == Node::NodeKind::BOOL_LITERAL_NODE) {
        return ConstantInt::getSigned(IntegerType::get(*TheContext, 1), static_pointer_cast<BoolLiteralNode>(node)->value);
    } else if (node->kind == Node::NodeKind::CHAR_LITERAL_NODE) {
        return ConstantInt::getSigned(IntegerType::get(*TheContext, 8), static_pointer_cast<CharLiteralNode>(node)->str[0]);
    } else if (node->kind == Node::NodeKind::FLOAT_LITERAL_NODE) {
        if (static_pointer_cast<FloatLiteralNode>(node)->doubleVal) {
            return ConstantFP::get(Type::getDoubleTy(*TheContext), static_pointer_cast<FloatLiteralNode>(node)->value);
        } else {
            return ConstantFP::get(Type::getFloatTy(*TheContext), static_pointer_cast<FloatLiteralNode>(node)->value);
        }
    }
}

Value* CodeGen::genMethodCall(shared_ptr<MethodCallNode> node, Value *calle) {
    if (node->record->similar.empty()) {
        string fullName = getFullMethodRecordName(node->record);
        Function *TheFunction = TheModule->getFunction(fullName);
        vector<Value*> args = vector<Value*>();
        if (calle != nullptr) {
            args.push_back(calle);
            thisV.push(calle);
        }
        for (shared_ptr<ExpressionNode> arg : node->args) {
            args.push_back(genExpression(arg));
        }
        auto tmp =  Builder->CreateCall(TheFunction, args, "calltmp");
        if (calle != nullptr) {
            thisV.pop();
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
            string fullName = getFullMethodRecordName(funRecord);
            Function *TheFunction = TheModule->getFunction(fullName);
            bool same = true;
            for (int i = 0; i < args.size(); ++i) {
                if (args[i]->getType() != TheFunction->getArg(i)->getType()) {
                    same = false;
                }
            }
            if (same) {
                auto tmp = Builder->CreateCall(TheFunction, args, "calltmp");
                if (calle != nullptr) {
                    thisV.pop();
                }
                return tmp;
            }
        }
        auto funRecord = node->record;
        string fullName = getFullMethodRecordName(funRecord);
        Function *TheFunction = TheModule->getFunction(fullName);
        bool same = true;
        for (int i = 0; i < args.size(); ++i) {
            if (args[i]->getType() != TheFunction->getArg(i)->getType()) {
                same = false;
            }
        }
        if (same) {
            auto tmp = Builder->CreateCall(TheFunction, args, "calltmp");
            if (calle != nullptr) {
                thisV.pop();
            }
            return tmp;
        }

        Out::errorMessage("Fatal error! Can not car record of method call. Undefined reference: " + node->record->id);
    }
    
}

Value* CodeGen::genVarDecl(shared_ptr<VarDeclNode> node) {
    Value *val = node->init != nullptr ? genExpression(node->init) : genDefaultValue(node->type);
    Value *ptr = Builder->CreateAlloca(val->getType(), nullptr, getFullVarDeclNodeName(node));
    Builder->CreateStore(val, ptr);
    NamedValues[getFullVarDeclNodeName(node)] = ptr;
    varTypes[node->record] = val->getType();
}

Value* CodeGen::genDefaultValue(shared_ptr<TypeNode> node) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "boolean") {
            return ConstantInt::get(IntegerType::get(*TheContext, 32), 0);
        } else if (x->record->id == "int") {
            return ConstantInt::get(IntegerType::get(*TheContext, 32), 0);
        } else if (x->record->id == "byte") {
            return ConstantInt::get(IntegerType::get(*TheContext, 8), 0);
        } else if (x->record->id == "short") {
            return ConstantInt::get(IntegerType::get(*TheContext, 16), 0);
        } else if (x->record->id == "long") {
            return ConstantInt::get(IntegerType::get(*TheContext, 64), 0);
        } else if (x->record->id == "float") {
            return ConstantFP::get(Type::getFloatTy(*TheContext), 0);
        } else if (x->record->id == "double") {
            return ConstantFP::get(Type::getDoubleTy(*TheContext), 0);
        } else if (x->record->id == "char") {
            return ConstantInt::get(IntegerType::get(*TheContext, 8), 0);
        } else if (x->record->id == "void") {
            return nullptr;
        }
    } else {
        StructType *structType;
        if (classesTypes.contains(getFullClassRecordName(x->record))) {
            structType = classesTypes.at(getFullClassRecordName(x->record));
        } else {
            structType = nullptr;
            Out::errorMessage("Can not get " + getFullClassRecordName(x->record));
        }
        PointerType *structPtrType = PointerType::get(structType, 0);
        return ConstantPointerNull::get(structPtrType);
    }
}

Value* CodeGen::genVarValue(shared_ptr<VarRecordNode> node) {
    Value *ptr = NamedValues[getFullVarRecordName(node->record)];
    if (ptr == nullptr) {
        ptr = GlobalNamedValues[getFullVarRecordName(node->record)];
    }
    Type *type = varTypes[node->record];
    Value *val = Builder->CreateLoad(type, ptr, "loadtmp");
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
            
            Value *last = nullptr;
            Value *last_ptr = nullptr;
            if (access->isExpression()) {
                for (int i = 0; i < access->access.size(); ++i) {
                    auto n = access->access[i];
                    if (i == 0) {
                        if (n->isExpression()) {
                            last = genExpression(static_pointer_cast<ExpressionNode>(n));
                            if (n->kind == Node::NodeKind::VAR_RECORD_NODE) {
                                last_ptr = NamedValues[getFullVarRecordName(static_pointer_cast<VarRecordNode>(n)->record)];
                            }
                            continue;
                        } else {
                            Out::errorMessage("Internall error detected: can not generate expression.");
                            break;
                        }
                    }
                    
                    if (n->isExpression()) {
                        auto last_n = access->access[i-1];
                        if (n->kind == Node::NodeKind::VAR_RECORD_NODE) {
                            shared_ptr<VarRecord> n_var_rec = static_pointer_cast<VarRecordNode>(n)->record;
                            if (last_n->isExpression()) {
                                auto classRecord = static_pointer_cast<ExpressionNode>(last_n)->getReturnType();
                                shared_ptr<TypeNode> typeNode = make_shared<TypeNode>(make_shared<ClassRecordNode>(classRecord, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr);
                                Type *t = getType(typeNode, false);
                                int struct_n = 0;
                                for (int j = 0; j < classRecord->fields.size(); ++j) {
                                    if (classRecord->fields[j]->equals(n_var_rec)) {
                                        struct_n = j;
                                        break;
                                    }
                                }
                                Value *nullV = ConstantInt::getSigned(IntegerType::get(*TheContext, 32), 0);
                                Value *struct_nV = ConstantInt::getSigned(IntegerType::get(*TheContext, 32), struct_n);
                                Value *getelementptr = GetElementPtrInst::Create(t, last, vector<Value*>{nullV, struct_nV}, "access_tmp", Builder->GetInsertBlock());
                                
                                auto n_classRecord = static_pointer_cast<ExpressionNode>(n)->getReturnType();
                                shared_ptr<TypeNode> n_typeNode = make_shared<TypeNode>(make_shared<ClassRecordNode>(n_classRecord, vector<shared_ptr<AccessNode>>(), nullptr), 0, nullptr);
                                Type *n_t = getType(n_typeNode, false);
                                last = Builder->CreateLoad(n_t, getelementptr, "loadgetelementptrtmp");
                                last_ptr = getelementptr;
                            } else {
                                Out::errorMessage("Internall error detected: can not generate expression.");
                                break;
                            }
                        } else if (n->kind == Node::NodeKind::METHOD_CALL_NODE) {
                            shared_ptr<MethodCallNode> callNode = static_pointer_cast<MethodCallNode>(n);
                            bool isStatic = false;
                            for (auto mod : callNode->record->mods) {
                                if (mod == ModifiersNode::ModifierKind::STATIC) {
                                    isStatic = true;
                                    break;
                                }
                            }
                            if (isStatic) {
                                Out::errorMessage("Error: call of static method with non class access.");
                            } else {
                                last = genMethodCall(callNode, last);
                            }
                        }
                        continue;
                    } else {
                        Out::errorMessage("Internall error detected: can not generate expression.");
                        break;
                    }
                }
            }
            
            Builder->CreateStore(R, last_ptr);
        } else {
            Out::errorMessage("This assign kind is currently unsupported.");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::OR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::AND) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_OR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_AND) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::EQUAL) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OEQ, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpeqtmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_EQ, L, R, "eqtmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_ONE, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpneqtmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_NE, L, R, "neqtmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OLT, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fplttmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_SLT, L, R, "lttmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OGT, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpgttmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_SGT, L, R, "gttmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OLE, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpletmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_SLE, L, R, "letmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OGE, Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpgetmp"); 
        } else {
            return Builder->CreateICmp(CmpInst::Predicate::ICMP_SGE, L, R, "getmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT) {
        
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFAdd(Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpaddtmp"); 
        } else {
            return Builder->CreateAdd(L, R, "addtmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFSub(Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpsubtmp"); 
        } else {
            return Builder->CreateSub(L, R, "subtmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFMul(Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpmultmp"); 
        } else {
            return Builder->CreateMul(L, R, "multmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV) {
        if (L->getType()->isDoubleTy() || L->getType()->isFloatTy() ||
            R->getType()->isDoubleTy() || R->getType()->isFloatTy()) {

            return Builder->CreateFDiv(Builder->CreateFPCast(L, Type::getDoubleTy(*TheContext), "fpcast"),
                                       Builder->CreateFPCast(R, Type::getDoubleTy(*TheContext), "fpcast"),
                                       "fpdivtmp"); 
        } else {
            return Builder->CreateSDiv(L, R, "divtmp");
        }
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD) {
        
    }
}

Value* CodeGen::genNewNode(shared_ptr<NewNode> node) {
    Type *type = getType(node->type);
    Value *one = ConstantInt::getSigned(IntegerType::get(*TheContext, 32), 1);
    Value *sizeofV = GetElementPtrInst::Create(type, ConstantPointerNull::get(PointerType::get(*TheContext, 0)), vector<Value *>{one}, "sizeof", Builder->GetInsertBlock());
    Value *sizeofIV = Builder->CreatePtrToInt(sizeofV, IntegerType::get(*TheContext, 32), "sizeofI");

    Function *splMallocFunction = TheModule->getFunction("__spl__malloc");
    return Builder->CreateCall(splMallocFunction, vector<Value *>{sizeofIV}, "heapallocatmp");
}
