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

using namespace llvm;

class CodeGen {
public:
    shared_ptr<LLVMContext> TheContext;
    shared_ptr<IRBuilder<>> Builder;
    shared_ptr<Module> TheModule;
    map<string, Value *> NamedValues;
    map<string, Value *> GlobalNamedValues;
    map<Value *, shared_ptr<ExpressionNode>> StaticGlobalsInit;
    map<shared_ptr<VarRecord>, Type *> varTypes;
    BasicBlock *retBB;
    stack<Value*> thisV;

    shared_ptr<CompilationUnitNode> cu;
    shared_ptr<ClassDeclNode> currClass;
    string currClassName = "";
    stack<shared_ptr<ClassDeclNode>> classesStack = stack<shared_ptr<ClassDeclNode>>();
    map<string, StructType*> classesTypes = map<string, StructType*>();

    CodeGen(shared_ptr<CompilationUnitNode> _cu);

    void codeGen();

    void build();
    void genImport(shared_ptr<ImportDeclNode> node);
    void createClassType(shared_ptr<ClassDeclNode> node);
    void genClassDecl(shared_ptr<ClassDeclNode> node, bool genMethod);
    void setCurrClassName();
    string getFullClassRecordName(shared_ptr<ClassRecord> rec);
    string getFullMethodDeclNodeName(shared_ptr<MethodDeclNode> node);
    string getFullVarDeclNodeName(shared_ptr<VarDeclNode> node);
    string getFullMethodRecordName(shared_ptr<MethodRecord> rec);
    string getFullVarRecordName(shared_ptr<VarRecord> rec);
    void genStruct(shared_ptr<ClassDeclNode> node);
    Type* getType(shared_ptr<TypeNode> node, bool ptr);
    Function* genMethodPrototype(shared_ptr<MethodDeclNode> node);
    Function* genMethodDecl(shared_ptr<MethodDeclNode> node);
    bool genBlockStatement(shared_ptr<BlockNode> node);
    void genIfElse(shared_ptr<IfElseNode> node);
    void genWhile(shared_ptr<WhileNode> node);
    void genFor(shared_ptr<ForNode> node);
    Value* genExpression(shared_ptr<ExpressionNode> node);
    Value* genLiteral(shared_ptr<ExpressionNode> node);
    Value* genMethodCall(shared_ptr<MethodCallNode> node, Value *calle);
    Value* genVarDecl(shared_ptr<VarDeclNode> node);
    Value* genDefaultValue(shared_ptr<TypeNode> node);
    Value* genVarValue(shared_ptr<VarRecordNode> node);
    Value* genBinOp(shared_ptr<BinaryOperatorNode> node);
    Value* genNewNode(shared_ptr<NewNode> node);
};
