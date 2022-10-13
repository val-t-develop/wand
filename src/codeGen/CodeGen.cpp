#include "CodeGen.hpp"
#include <utils/Out.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
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
            moduleName += str + ".";
        }
        moduleName.pop_back();
    }

    TheModule = make_shared<Module>(moduleName, *TheContext);
    Builder = make_shared<IRBuilder<>>(*TheContext);
}

void CodeGen::codeGen() {
    
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
        if (node->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::IMPORT_DECL_NODE) {
            continue;
        } else if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            genClassDecl(static_pointer_cast<ClassDeclNode>(node));
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

void CodeGen::genImport(shared_ptr<ImportDeclNode> node) {}

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

void CodeGen::genClassDecl(shared_ptr<ClassDeclNode> node) {
    currClass = node;
    classesStack.push(node);
    setCurrClassName();


    genStruct(node);

    for (shared_ptr<ClassDeclNode> item : node->innerClasses) {
        genClassDecl(item);
    }

    for (shared_ptr<MethodDeclNode> item : node->methods) {
        genMethodDecl(item);
    }

    classesStack.pop();
    currClass = classesStack.empty() ? nullptr : classesStack.top();
    setCurrClassName();
}

void CodeGen::setCurrClassName() {
    if (currClass != nullptr) {
        string str = getFullRecordName(currClass->record);
        currClassName = str;
    } else {
        currClassName = "";
    }
}

string CodeGen::getFullRecordName(shared_ptr<Record> rec) {
    /*string str;
    while (true) {
        if (rec != nullptr) {
            str = rec->id + "." + str;
            rec = rec->next;
        } else {
            str.pop_back();
            break;
        }
    }
    return str;*/
    string str = "";
    if (rec->next != nullptr) {
        str += getFullRecordName(rec->next) + ".";
    }
    str += rec->id;
    return str;
}

void CodeGen::genStruct(shared_ptr<ClassDeclNode> node) {
    vector<Type*> types = vector<Type*>();

    for (shared_ptr<VarDeclNode> var : node->fields) {
        types.push_back(getType(var->type));
    }
    string fullName = getFullRecordName(node->record);

    StructType *structType;
    if (classesTypes.contains(fullName)) {
        structType = classesTypes.at(fullName);
    } else {
        structType = nullptr;
        Out::errorMessage("Can not get " + fullName);
    }
    structType->setBody(types);
}

Type* CodeGen::getType(shared_ptr<TypeNode> node) {
    auto x = static_pointer_cast<ClassRecordNode>(node->type->child);
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
        StructType *structType;
        if (classesTypes.contains(getFullRecordName(x->record))) {
            structType = classesTypes.at(getFullRecordName(x->record));
        } else {
            structType = nullptr;
            Out::errorMessage("Can not get " + getFullRecordName(x->record));
        }
        PointerType *structPtrType = PointerType::get(structType, 0);
        return structPtrType;
    }
}

Function* CodeGen::genMethodPrototype(shared_ptr<MethodDeclNode> node) {
    Function *TheFunction = TheModule->getFunction(getFullRecordName(node->record));

    vector<Type*> args_types = vector<Type*>();
    if (!TheFunction) {
        
        for (shared_ptr<VarDeclNode> arg : node->args) {
            args_types.push_back(getType(arg->type));
        }
        FunctionType* ft = FunctionType::get(getType(node->returnType), args_types, false);

        TheFunction = Function::Create(ft, Function::ExternalLinkage, getFullRecordName(node->record), *TheModule);
    }

    if (!TheFunction) {
        return nullptr;
    }
    return TheFunction;
}

Function* CodeGen::genMethodDecl(shared_ptr<MethodDeclNode> node) {

    Function *TheFunction = TheModule->getFunction(getFullRecordName(node->record));

    vector<Type*> args_types = vector<Type*>();
    for (shared_ptr<VarDeclNode> arg : node->args) {
        args_types.push_back(getType(arg->type));
    }

    if (!TheFunction) {
        return nullptr;
    }
    
    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[string(Arg.getName())] = &Arg;

    if (Value *RetVal = genBlockStatement(node->body)) {
        Builder->CreateRet(RetVal);
        verifyFunction(*TheFunction);


        if (static_pointer_cast<ClassRecordNode>(node->returnType->type->child)->record->id == "int") {
            if (node->record->id == "main") {
                if (node->args.size() == 1) {
                    if (static_pointer_cast<ClassRecordNode>(node->args.at(0)->type->type->child)->record->id == "String") {
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
    } else {
        RetVal = ReturnInst::Create(*TheContext);
        Builder->CreateRet(RetVal);
        verifyFunction(*TheFunction);


        if (static_pointer_cast<ClassRecordNode>(node->returnType->type->child)->record->id == "int") {
            if (node->record->id == "main") {
                if (node->args.size() == 1) {
                    if (static_pointer_cast<ClassRecordNode>(node->args.at(0)->type->type->child)->record->id == "String") {
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

Value* CodeGen::genBlockStatement(shared_ptr<BlockNode> node) {
    for (shared_ptr<Node> item : node->nodes) {
        if (item->kind == Node::NodeKind::RETURN_NODE) {
            return genExpression(static_pointer_cast<ReturnNode>(item)->expression);
        } else if (item->kind == Node::NodeKind::VAR_DECL_NODE) {
            genVarDecl(static_pointer_cast<VarDeclNode>(item));
        } else if (item->kind == Node::NodeKind::VARS_DECL_NODE) {
            for (shared_ptr<VarDeclNode> decl : static_pointer_cast<VarsDeclNode>(item)->decls) {
                genVarDecl(decl);
            }
        } else if (item->isExpression()) {
            genExpression(static_pointer_cast<ExpressionNode>(item));
        }
    }
    return nullptr;
}

Value* CodeGen::genExpression(shared_ptr<ExpressionNode> node) {
    if (node->isLiteral()) {
        return genLiteral(node);
    } else if (node->kind == Node::NodeKind::METHOD_CALL_NODE) {
        return genMethodCall(static_pointer_cast<MethodCallNode>(node));
    } else if (node->kind == Node::NodeKind::VAR_RECORD_NODE) {
        return genVarValue(static_pointer_cast<VarRecordNode>(node));
    } else if (node->kind == Node::NodeKind::ACCESS_NODE) {
        return genExpression(static_pointer_cast<ExpressionNode>(static_pointer_cast<AccessNode>(node)->child));
    }
}

Value* CodeGen::genLiteral(shared_ptr<ExpressionNode> node) {
    if (node->kind == Node::NodeKind::INT_LITERAL_NODE) {
        return ConstantInt::getSigned(IntegerType::get(*TheContext, 32), static_pointer_cast<IntLiteralNode>(node)->value);
    }
}

Value* CodeGen::genMethodCall(shared_ptr<MethodCallNode> node) {
    string fullName = getFullRecordName(node->record);
    Function *TheFunction = TheModule->getFunction(fullName);
    vector<Value*> args = vector<Value*>();
    for (shared_ptr<ExpressionNode> arg : node->args) {
        args.push_back(genExpression(arg));
    }
    return Builder->CreateCall(TheFunction, args, "");
}

Value* CodeGen::genVarDecl(shared_ptr<VarDeclNode> node) {
    NamedValues[getFullRecordName(node->record)] = node->init != nullptr ? genExpression(node->init) : genDefaultValue(node->type);
}

Value* CodeGen::genDefaultValue(shared_ptr<TypeNode> node) {
    auto x = static_pointer_cast<ClassRecordNode>(node->type->child);
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
        if (classesTypes.contains(getFullRecordName(x->record))) {
            structType = classesTypes.at(getFullRecordName(x->record));
        } else {
            structType = nullptr;
            Out::errorMessage("Can not get " + getFullRecordName(x->record));
        }
        PointerType *structPtrType = PointerType::get(structType, 0);
        return ConstantPointerNull::get(structPtrType);
    }
}

Value* CodeGen::genVarValue(shared_ptr<VarRecordNode> node) {
    return NamedValues[getFullRecordName(node->record)];
}

