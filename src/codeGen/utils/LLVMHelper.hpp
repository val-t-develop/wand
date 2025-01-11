/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022-2024  Valentyn Tymchyshyn
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
 *  Useful interface for interacting with llvm.
 */

#pragma once
#include "llvm/IR/DIBuilder.h"
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
#include <llvm/Target/TargetMachine.h>

#include <IRTree/node/IRFunction.hpp>
#include <IRTree/node/IRStruct.hpp>

using namespace llvm;

class LLVMHelper {
public:
    map<string, DIType*> DTypes;
    shared_ptr<LLVMContext> TheContext;
    shared_ptr<IRBuilder<>> Builder;
    shared_ptr<Module> TheModule;

    shared_ptr<DIBuilder> DBuilder = nullptr;
    DICompileUnit* DCU = nullptr;

    TargetMachine *TheTargetMachine;

    LLVMHelper(string moduleName, string path);

    string getModuleName();
    void createDTypes();
    StructType *createStructType(shared_ptr<IRStruct> node);
    GlobalVariable *createGlobalVar(Type *type, string name);
    GlobalVariable *createConstantVar(Type *type, string name, Constant *init);
    Function *createFunctionPrototype(string name, Type *ret,
                                      vector<Type *> args);
    Function *getFunction(string name);

    BasicBlock *createBBinFunc(string name, Function *func);
    void activateBB(BasicBlock *BB);
    BasicBlock *getActiveBB();
    Function *getCurrFunction();

    Value *createAlloca(Type *type, Value *arrSize, string name = "allocatmp");
    void createStore(Value *val, Value *ptr);
    void createBr(BasicBlock *BB);
    void createRet(Value *val);
    void createRet();
    Value *createCall(Function *func, vector<Value *> args = vector<Value *>(),
                      string name = "calltmp");
    Value *createCall(string func, vector<Value *> args = vector<Value *>(),
                      string name = "calltmp");
    Value *createLoad(Type *type, Value *ptr, string name = "loadtmp");
    void createIfElse(Value *cond, BasicBlock *thenBB, BasicBlock *elseBB);
    Value *createPtrToInt(Value *ptr, Type *intType,
                          string name = "ptrtointtmp");
    Value *createGetElementPtr(Type *type, Value *ptr, vector<Value *> indexes,
                               string name = "getelementptrtmp");

    Value *createEQ(Value *lhs, Value *rhs, string name = "eqtmp");
    Value *createFPEQ(Value *lhs, Value *rhs, string name = "fpeqtmp");
    Value *createNE(Value *lhs, Value *rhs, string name = "netmp");
    Value *createFPNE(Value *lhs, Value *rhs, string name = "fpnetmp");
    Value *createLT(Value *lhs, Value *rhs, string name = "lttmp");
    Value *createFPLT(Value *lhs, Value *rhs, string name = "fplttmp");
    Value *createGT(Value *lhs, Value *rhs, string name = "gttmp");
    Value *createFPGT(Value *lhs, Value *rhs, string name = "fpgttmp");
    Value *createLE(Value *lhs, Value *rhs, string name = "letmp");
    Value *createFPLE(Value *lhs, Value *rhs, string name = "fpletmp");
    Value *createGE(Value *lhs, Value *rhs, string name = "getmp");
    Value *createFPGE(Value *lhs, Value *rhs, string name = "fpgetmp");
    Value *createAdd(Value *lhs, Value *rhs, string name = "addtmp");
    Value *createFPAdd(Value *lhs, Value *rhs, string name = "fpaddtmp");
    Value *createSub(Value *lhs, Value *rhs, string name = "subtmp");
    Value *createFPSub(Value *lhs, Value *rhs, string name = "fpsubtmp");
    Value *createMul(Value *lhs, Value *rhs, string name = "multmp");
    Value *createFPMul(Value *lhs, Value *rhs, string name = "fpmultmp");
    Value *createDiv(Value *lhs, Value *rhs, string name = "divtmp");
    Value *createFPDiv(Value *lhs, Value *rhs, string name = "fpdivtmp");

    Value *castToDouble(Value *val, string name = "doublecast");
    Value *castToFloat(Value *val, string name = "floatcast");

    ConstantInt *getConstInt(int size, uint64_t val);
    Constant *getConstFloat(double val);
    Constant *getConstDouble(double val);
    Constant *getConstString(string val);
    Constant *getConstNullTerminatedString(string val);
    ConstantPointerNull *getNullptr(PointerType *type);

    IntegerType *getIntType(int size);
    Type *getFloatType();
    Type *getDoubleType();
    PointerType *getPointerType(Type *type, int addressSpace = 0);
    Type *getVoidType();
    ArrayType *getArrayType(Type *type, uint64_t n);

    bool isFloatValue(Value *val);
    bool isDoubleValue(Value *val);
    bool isFloatingPointValue(Value *val);
    bool isIntValue(Value *val);

    Value *createSizeof(Type *type);

    void printModule(raw_ostream& stream);
    void prepareBuild();
    void runPasses();
    void build(raw_pwrite_stream &dest);

    void setDebugLocation(int line, int col);
};
