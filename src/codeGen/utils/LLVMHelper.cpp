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

#include "LLVMHelper.hpp"
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <ast/node/statement/expression/literal/NullNode.hpp>
#include <llvm/Support/CodeGen.h>
#include <main.hpp>
#include <utils/Out.hpp>

#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/PassBuilder.h>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

LLVMHelper::LLVMHelper(string moduleName, string path) {
    TheContext = make_shared<LLVMContext>();
    TheModule = make_shared<Module>(moduleName, *TheContext);
    Builder = make_shared<IRBuilder<>>(*TheContext);

    if (Main::debug) {
        DBuilder = make_shared<DIBuilder>(*TheModule);
        DCU = DBuilder->createCompileUnit(dwarf::DW_LANG_C, DBuilder->createFile(path, "/"), "SPL Compiler", false, "", 0);
    }
}

Type *LLVMHelper::getVoidType() { return Type::getVoidTy(*TheContext); }

void LLVMHelper::prepareBuild() {
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

    string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!Target) {
        errs() << Error;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = std::optional<Reloc::Model>();
    TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());
    if (Main::debug) {
        DBuilder->finalize();
    }
}

void LLVMHelper::printModule() { TheModule->print(errs(), nullptr); }

void LLVMHelper::runPasses() {
    // Create the analysis managers.
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;
    PassBuilder PB{TheTargetMachine};
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
    ModulePassManager MPM;
    if (Main::optLevel==Main::OptLevel::O0) {
        PB.buildPerModuleDefaultPipeline(OptimizationLevel::O0);
    } else if (Main::optLevel==Main::OptLevel::O1) {
        PB.buildPerModuleDefaultPipeline(OptimizationLevel::O1);
    } else if (Main::optLevel==Main::OptLevel::O2) {
        PB.buildPerModuleDefaultPipeline(OptimizationLevel::O2);
    } else if (Main::optLevel==Main::OptLevel::O3) {
        PB.buildPerModuleDefaultPipeline(OptimizationLevel::O3);
    }
    MPM.run(*TheModule, MAM);
}

void LLVMHelper::build(raw_pwrite_stream &dest) {
    legacy::PassManager pass;
    auto FileType = CodeGenFileType::ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType, false)) {
        errs() << "TheTargetMachine can't emit a file of this type";
    }

    pass.run(*TheModule);
    dest.flush();
}

void LLVMHelper::createDTypes() {
    DTypes["bool"] = DBuilder->createBasicType("bool", 8, dwarf::DW_ATE_signed);
    DTypes["char"] = DBuilder->createBasicType("char", 8, dwarf::DW_ATE_signed_char);
    DTypes["byte"] = DBuilder->createBasicType("byte", 8, dwarf::DW_ATE_signed);
    DTypes["short"] = DBuilder->createBasicType("short", 16, dwarf::DW_ATE_signed);
    DTypes["int"] = DBuilder->createBasicType("int", 32, dwarf::DW_ATE_signed);
    DTypes["long"] = DBuilder->createBasicType("long", 64, dwarf::DW_ATE_signed);
    DTypes["float"] = DBuilder->createBasicType("float", getFloatType()->getPrimitiveSizeInBits(), dwarf::DW_ATE_float);
    DTypes["double"] = DBuilder->createBasicType("double", getDoubleType()->getPrimitiveSizeInBits(), dwarf::DW_ATE_float);
    DTypes["void"] = DBuilder->createBasicType("void", 0, dwarf::DW_ATE_address);
}

StructType *LLVMHelper::createStructType(shared_ptr<IRStruct> node) {
    if (DBuilder!=nullptr) {
        DTypes[node->name] = DBuilder->createReplaceableCompositeType(dwarf::DW_TAG_structure_type, node->name, DCU->getFile(), DCU->getFile(), node->line);
    }
    return StructType::create(*TheContext, node->name);
}

GlobalVariable *LLVMHelper::createGlobalVar(Type *type, string name) {
    return new GlobalVariable(*TheModule, type, false,
                              GlobalValue::LinkageTypes::ExternalLinkage, 0,
                              name);
}

GlobalVariable *LLVMHelper::createConstantVar(Type *type, string name, Constant *init) {
    return new GlobalVariable(*TheModule, type, true,
                              GlobalValue::LinkageTypes::ExternalLinkage, init,
                              name);
}

ConstantPointerNull *LLVMHelper::getNullptr(PointerType *type) {
    return ConstantPointerNull::get(type);
}

Function *LLVMHelper::getFunction(string name) {
    return TheModule->getFunction(name);
}

Function *LLVMHelper::createFunctionPrototype(string name, Type *ret,
                                              vector<Type *> args) {
    Function *f = getFunction(name);
    if (!f) {
        FunctionType *ft = FunctionType::get(ret, args, false);
        f = Function::Create(ft, Function::ExternalLinkage, name,
                                *TheModule);
    }
    return f;
}

BasicBlock *LLVMHelper::createBBinFunc(string name, Function *func) {
    BasicBlock *BB = BasicBlock::Create(*TheContext, name, func);
    return BB;
}

void LLVMHelper::activateBB(BasicBlock *BB) { Builder->SetInsertPoint(BB); }

Value *LLVMHelper::createAlloca(Type *type, Value *arrSize, string name) {
    return Builder->CreateAlloca(type, arrSize, name);
}

void LLVMHelper::createStore(Value *val, Value *ptr) {
    Builder->CreateStore(val, ptr);
}

void LLVMHelper::createBr(BasicBlock *BB) { Builder->CreateBr(BB); }

void LLVMHelper::createRet(Value *val) { Builder->CreateRet(val); }

void LLVMHelper::createRet() { Builder->CreateRetVoid(); }

Value *LLVMHelper::createCall(Function *func, vector<Value *> args,
                              string name) {
    if (func->getReturnType()==getVoidType()) {
        return Builder->CreateCall(func, args);
    } else {
        return Builder->CreateCall(func, args, name);
    }
}

Value *LLVMHelper::createCall(string func, vector<Value *> args, string name) {
    Function *f = getFunction(func);
    if (!f)
        Out::errorMessage("Can not create call instruction for " + func);
    return createCall(f, args, name);
}

Value *LLVMHelper::createLoad(Type *type, Value *ptr, string name) {
    return Builder->CreateLoad(type, ptr, name);
}

void LLVMHelper::createIfElse(Value *cond, BasicBlock *thenBB,
                              BasicBlock *elseBB) {
    Builder->CreateCondBr(cond, thenBB, elseBB);
}

BasicBlock *LLVMHelper::getActiveBB() { return Builder->GetInsertBlock(); }

Function *LLVMHelper::getCurrFunction() { return getActiveBB()->getParent(); }

IntegerType *LLVMHelper::getIntType(int size) {
    return IntegerType::get(*TheContext, size);
}

ConstantInt *LLVMHelper::getConstInt(int size, uint64_t val) {
    return ConstantInt::get(getIntType(size), val, true);
}

Type *LLVMHelper::getFloatType() { return Type::getFloatTy(*TheContext); }

Type *LLVMHelper::getDoubleType() { return Type::getDoubleTy(*TheContext); }

Constant *LLVMHelper::getConstFloat(double val) {
    return ConstantFP::get(getFloatType(), val);
}

Constant *LLVMHelper::getConstDouble(double val) {
    return ConstantFP::get(getDoubleType(), val);
}

Constant *LLVMHelper::getConstString(string val) {
    return ConstantDataArray::getString(*TheContext, val, false);
}

Constant *LLVMHelper::getConstNullTerminatedString(string val) {
    return ConstantDataArray::getString(*TheContext, val, true);
}

PointerType *LLVMHelper::getPointerType(Type *type, int addressSpace) {
    return PointerType::get(type, addressSpace);
}

Value *LLVMHelper::createPtrToInt(Value *ptr, Type *intType, string name) {
    return Builder->CreatePtrToInt(ptr, intType, name);
}

Value *LLVMHelper::createGetElementPtr(Type *type, Value *ptr,
                                       vector<Value *> indexes, string name) {
    return GetElementPtrInst::Create(type, ptr, indexes, name, getActiveBB());
}

Value *LLVMHelper::createSizeof(Type *type) {
    return ConstantExpr::getSizeOf(type);
}

Value *LLVMHelper::createEQ(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_EQ, lhs, rhs, name);
}

Value *LLVMHelper::createFPEQ(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OEQ, lhs, rhs, name);
}

Value *LLVMHelper::createNE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_NE, lhs, rhs, name);
}

Value *LLVMHelper::createFPNE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_ONE, lhs, rhs, name);
}

Value *LLVMHelper::createLT(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_SLT, lhs, rhs, name);
}

Value *LLVMHelper::createFPLT(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OLT, lhs, rhs, name);
}

Value *LLVMHelper::createGT(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_SGT, lhs, rhs, name);
}

Value *LLVMHelper::createFPGT(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OGT, lhs, rhs, name);
}

Value *LLVMHelper::createLE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_SLE, lhs, rhs, name);
}

Value *LLVMHelper::createFPLE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OLE, lhs, rhs, name);
}

Value *LLVMHelper::createGE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateICmp(CmpInst::Predicate::ICMP_SGE, lhs, rhs, name);
}

Value *LLVMHelper::createFPGE(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFCmp(CmpInst::Predicate::FCMP_OGE, lhs, rhs, name);
}

Value *LLVMHelper::createAdd(Value *lhs, Value *rhs, string name) {
    return Builder->CreateAdd(lhs, rhs, name);
}

Value *LLVMHelper::createFPAdd(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFAdd(lhs, rhs, name);
}

Value *LLVMHelper::createSub(Value *lhs, Value *rhs, string name) {
    return Builder->CreateSub(lhs, rhs, name);
}

Value *LLVMHelper::createFPSub(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFSub(lhs, rhs, name);
}

Value *LLVMHelper::createMul(Value *lhs, Value *rhs, string name) {
    return Builder->CreateMul(lhs, rhs, name);
}

Value *LLVMHelper::createFPMul(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFMul(lhs, rhs, name);
}

Value *LLVMHelper::createDiv(Value *lhs, Value *rhs, string name) {
    return Builder->CreateSDiv(lhs, rhs, name);
}

Value *LLVMHelper::createFPDiv(Value *lhs, Value *rhs, string name) {
    return Builder->CreateFDiv(lhs, rhs, name);
}

bool LLVMHelper::isFloatValue(Value *val) {
    return val->getType()->isFloatTy();
}

bool LLVMHelper::isDoubleValue(Value *val) {
    return val->getType()->isDoubleTy();
}

bool LLVMHelper::isFloatingPointValue(Value *val) {
    return isFloatValue(val) || isDoubleValue(val);
}

bool LLVMHelper::isIntValue(Value *val) {
    return val->getType()->isIntegerTy();
}

Value *LLVMHelper::castToDouble(Value *val, string name) {
    return Builder->CreateFPCast(val, getDoubleType(), name);
}

Value *LLVMHelper::castToFloat(Value *val, string name) {
    return Builder->CreateFPCast(val, getFloatType(), name);
}

string LLVMHelper::getModuleName() { return string(TheModule->getName()); }

ArrayType *LLVMHelper::getArrayType(Type *type, uint64_t n) {
    return ArrayType::get(type, n);
}

void LLVMHelper::setDebugLocation(int line, int col) {
    if (DBuilder!=nullptr) {
        Builder->SetCurrentDebugLocation(DILocation::get(*TheContext, line, col, Builder->GetInsertBlock()->getParent()->getSubprogram()));
    }
}
