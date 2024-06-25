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
 *  Utilities used in CodeGen file.
 */

#include "CodeGenUtils.hpp"
#include <utils/Out.hpp>

CodeGenUtils::CodeGenUtils(CodeGen *_codeGen)
    : codeGen(_codeGen), helper(_codeGen->helper) {}

void CodeGenUtils::setCurrClassName() {
    if (currClass != nullptr) {
        currClassName = currClass->getFullName();
    } else {
        currClassName = "";
    }
}

Type *CodeGenUtils::getType(string id) {
    if (id == "bool") {
        return helper->getIntType(1);
    } else if (id == "int") {
        return helper->getIntType(32);
    } else if (id == "byte") {
        return helper->getIntType(8);
    } else if (id == "short") {
        return helper->getIntType(16);
    } else if (id == "long") {
        return helper->getIntType(64);
    } else if (id == "float") {
        return helper->getFloatType();
    } else if (id == "double") {
        return helper->getDoubleType();
    } else if (id == "char") {
        return helper->getIntType(8);
    } else if (id == "void") {
        return helper->getVoidType();
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(id)) {
            structType = classesTypes.at(id);
        } else {
            Out::errorMessage("Can not get " + id);
        }
        PointerType *structPtrType = helper->getPointerType(structType);
        return structPtrType;
    }
}

Type *CodeGenUtils::getTypeNoPtr(string id) {
    if (id == "bool") {
        return helper->getIntType(1);
    } else if (id == "int") {
        return helper->getIntType(32);
    } else if (id == "byte") {
        return helper->getIntType(8);
    } else if (id == "short") {
        return helper->getIntType(16);
    } else if (id == "long") {
        return helper->getIntType(64);
    } else if (id == "float") {
        return helper->getFloatType();
    } else if (id == "double") {
        return helper->getDoubleType();
    } else if (id == "char") {
        return helper->getIntType(8);
    } else if (id == "void") {
        return helper->getVoidType();
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(id)) {
            structType = classesTypes.at(id);
        } else {
            Out::errorMessage("Can not get " + id);
        }
        return structType;
    }
}

Type *CodeGenUtils::getType(shared_ptr<ClassRecord> node) {
    if (node->type == "primitive") {
        if (node->id == "bool") {
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

Type *CodeGenUtils::getTypeNoPtr(shared_ptr<ClassRecord> node) {
    if (node->type == "primitive") {
        if (node->id == "bool") {
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
    Type *recTy = getType(node->type->record);
    for (int i = 0; i < node->dims; ++i) {
        recTy = helper->getPointerType(recTy);
    }
    return recTy;
}

Type *CodeGenUtils::getTypeNoPtr(shared_ptr<TypeNode> node) {
    Type *recTy = getTypeNoPtr(node->type->record);
    return recTy;
}
void CodeGenUtils::destructAfterStatement() {
    for(auto v : codeGen->destructAfterStatement) {
         helper->createCall(
         "__spl__destroyvar",
         vector<Value *>{v.first, helper->getFunction("__spl__destructor__" +v.second)});
    }
    codeGen->destructAfterStatement.clear();
}
