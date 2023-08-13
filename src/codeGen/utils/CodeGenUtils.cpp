#include "CodeGenUtils.hpp"
#include <utils/Out.hpp>

CodeGenUtils::CodeGenUtils(CodeGen* _codeGen) : codeGen(_codeGen), helper(_codeGen->helper) {}

void CodeGenUtils::setCurrClassName() {
    if (currClass != nullptr) {
        currClassName = currClass->getFullName();
    } else {
        currClassName = "";
    }
}

Type* CodeGenUtils::getType(shared_ptr<ClassRecord> node) {
    if (node->type == "primitive") {
        if (node->id == "boolean") {
            return helper->getIntType(1);
        } else if (node->id == "int") {
            return helper->getIntType(32);
        } else if (node->id == "byte") {
            return helper->getIntType(8);
        } else if (node->id == "short") {
            return helper->getIntType(16);
        } else if (node->id == "long") {
            return helper->getIntType(64);
        } else if (node->id == "float") {
            return helper->getFloatType();
        } else if (node->id == "double") {
            return helper->getDoubleType();
        } else if (node->id == "char") {
            return helper->getIntType(8);
        } else if (node->id == "void") {
            return helper->getVoidType();
        }
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(node->getFullName())) {
            structType = classesTypes.at(node->getFullName());
        } else {
            Out::errorMessage("Can not get " + node->getFullName());
        }
        PointerType *structPtrType = helper->getPointerType(structType);
        return structPtrType;
    }
}

Type* CodeGenUtils::getTypeNoPtr(shared_ptr<ClassRecord> node) {
    if (node->type == "primitive") {
        if (node->id == "boolean") {
            return helper->getIntType(1);
        } else if (node->id == "int") {
            return helper->getIntType(32);
        } else if (node->id == "byte") {
            return helper->getIntType(8);
        } else if (node->id == "short") {
            return helper->getIntType(16);
        } else if (node->id == "long") {
            return helper->getIntType(64);
        } else if (node->id == "float") {
            return helper->getFloatType();
        } else if (node->id == "double") {
            return helper->getDoubleType();
        } else if (node->id == "char") {
            return helper->getIntType(8);
        } else if (node->id == "void") {
            return helper->getVoidType();
        }
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(node->getFullName())) {
            structType = classesTypes.at(node->getFullName());
        } else {
            Out::errorMessage("Can not get " + node->getFullName());
        }
        return structType;
    }
}

Type *CodeGenUtils::getType(shared_ptr<TypeNode> node) {
    Type* recTy = getType(node->type->record);
    for (int i = 0; i < node->dims; ++i) {
        recTy = helper->getPointerType(recTy);
    }
    return recTy;
}

Type *CodeGenUtils::getTypeNoPtr(shared_ptr<TypeNode> node) {
    Type* recTy = getTypeNoPtr(node->type->record);
    return recTy;
}


