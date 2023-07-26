#pragma once
#include <ast/builder/AstBuilder.hpp>
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
#include "LLVMHelper.hpp"
#include <codeGen/CodeGen.hpp>
#include "LLVMHelper.hpp"

using namespace llvm;

class CodeGen;

class CodeGenUtils {
public:
    CodeGen* codeGen;
    shared_ptr<LLVMHelper> helper;

    shared_ptr<ClassDeclNode> currClass;
    string currClassName = "";
    map<string, StructType*> classesTypes = map<string, StructType*>();


    CodeGenUtils(CodeGen* _codeGen);

    void setCurrClassName();
    Type* getType(shared_ptr<ClassRecord> node);
    Type* getTypeNoPtr(shared_ptr<ClassRecord> node);
};