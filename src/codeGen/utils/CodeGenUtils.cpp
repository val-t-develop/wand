/*  WAND - Wand Programming Language compiler
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Utilities used in CodeGen file.
 */

#include "CodeGenUtils.hpp"
#include <utils/Out.hpp>

CodeGenUtils::CodeGenUtils(CodeGen *_codeGen)
    : codeGen(_codeGen), helper(_codeGen->helper) {}

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

void CodeGenUtils::destructAfterStatement() {
    for(auto v : codeGen->destructAfterStatement) {
         helper->createCall(
         "__wand__destroyobj",
         vector<Value *>{v.val, helper->getFunction("__wand__destructor__" +v.type)});
    }
    codeGen->destructAfterStatement.clear();
}
