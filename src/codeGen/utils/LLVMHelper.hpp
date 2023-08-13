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
#include <llvm/Target/TargetMachine.h>

using namespace llvm;

class LLVMHelper {
public:
    shared_ptr<LLVMContext> TheContext;
    shared_ptr<IRBuilder<>> Builder;
    shared_ptr<Module> TheModule;

    TargetMachine *TheTargetMachine;

    LLVMHelper(string moduleName);

    string getModuleName();

    StructType* createStructType(string name);
    GlobalVariable* createGlobalVar(Type *type, string name);
    Function* createFunctionPrototype(string name, Type* ret, vector<Type*> args);
    Function* getFunction(string name);

    BasicBlock* createBBinFunc(string name, Function* func);
    void activateBB(BasicBlock* BB);
    BasicBlock* getActiveBB();
    Function* getCurrFunction();

    Value* createAlloca(Type* type, Value* arrSize, string name="allocatmp");
    void createStore(Value* val, Value* ptr);
    void createBr(BasicBlock* BB);
    void createRet(Value* val);
    Value* createCall(Function *func, vector<Value *> args=vector<Value *>(), string name="calltmp");
    Value* createCall(string func, vector<Value *> args=vector<Value *>(), string name="calltmp");
    Value* createLoad(Type* type, Value* ptr, string name="loadtmp");
    void createIfElse(Value* cond, BasicBlock* thenBB, BasicBlock* elseBB);
    Value* createPtrToInt(Value* ptr, Type* intType, string name="ptrtointtmp");
    Value* createGetElementPtr(Type *type, Value *ptr, vector<Value *> indexes, string name="getelementptrtmp");

    Value* createEQ(Value* lhs, Value* rhs, string name="eqtmp");
    Value* createFPEQ(Value* lhs, Value* rhs, string name="fpeqtmp");
    Value* createNE(Value* lhs, Value* rhs, string name="netmp");
    Value* createFPNE(Value* lhs, Value* rhs, string name="fpnetmp");
    Value* createLT(Value* lhs, Value* rhs, string name="lttmp");
    Value* createFPLT(Value* lhs, Value* rhs, string name="fplttmp");
    Value* createGT(Value* lhs, Value* rhs, string name="gttmp");
    Value* createFPGT(Value* lhs, Value* rhs, string name="fpgttmp");
    Value* createLE(Value* lhs, Value* rhs, string name="letmp");
    Value* createFPLE(Value* lhs, Value* rhs, string name="fpletmp");
    Value* createGE(Value* lhs, Value* rhs, string name="getmp");
    Value* createFPGE(Value* lhs, Value* rhs, string name="fpgetmp");
    Value* createAdd(Value* lhs, Value* rhs, string name="addtmp");
    Value* createFPAdd(Value* lhs, Value* rhs, string name="fpaddtmp");
    Value* createSub(Value* lhs, Value* rhs, string name="subtmp");
    Value* createFPSub(Value* lhs, Value* rhs, string name="fpsubtmp");
    Value* createMul(Value* lhs, Value* rhs, string name="multmp");
    Value* createFPMul(Value* lhs, Value* rhs, string name="fpmultmp");
    Value* createDiv(Value* lhs, Value* rhs, string name="divtmp");
    Value* createFPDiv(Value* lhs, Value* rhs, string name="fpdivtmp");

    Value* castToDouble(Value* val, string name="doublecast");
    Value* castToFloat(Value* val, string name="floatcast");

    ConstantInt* getConstInt(int size, uint64_t val);
    Constant* getConstFloat(double val);
    Constant* getConstDouble(double val);
    ConstantPointerNull* getNullptr(PointerType* type);

    IntegerType* getIntType(int size);
    Type* getFloatType();
    Type* getDoubleType();
    PointerType* getPointerType(Type* type, int addressSpace=0);
    Type* getVoidType();
    ArrayType* getArrayType(Type* type, uint64_t n);

    bool isFloatValue(Value* val);
    bool isDoubleValue(Value* val);
    bool isFloatingPointValue(Value* val);

    Value* createSizeof(Type* type);

    void printModule();
    void prepareBuild();
    void runPasses();
    void build(raw_pwrite_stream &dest);
};

