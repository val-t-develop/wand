#include "CodeGenUtils.hpp"
#include <utils/Out.hpp>

CodeGenUtils::CodeGenUtils(CodeGen* _codeGen) : codeGen(_codeGen) {}

void CodeGenUtils::setCurrClassName() {
    if (currClass != nullptr) {
        string str = getFullClassRecordName(currClass->record);
        currClassName = str;
    } else {
        currClassName = "";
    }
}

string CodeGenUtils::getFullClassRecordName(shared_ptr<ClassRecord> rec) {
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

string CodeGenUtils::getFullMethodDeclNodeName(shared_ptr<MethodDeclNode> node) {
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

string CodeGenUtils::getFullVarDeclNodeName(shared_ptr<VarDeclNode> node) {
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

string CodeGenUtils::getFullMethodRecordName(shared_ptr<MethodRecord> rec) {
    if (rec->ir_name == "") {
        Out::errorMessage("Internal error detected! Can not get ir_name of method " + rec->id);
    }
    return rec->ir_name;
}

string CodeGenUtils::getFullVarRecordName(shared_ptr<VarRecord> rec) {
    if (rec->ir_name == "") {
        Out::errorMessage("Internal error detected! Can not get ir_name of var or field " + rec->id);
    }
    return rec->ir_name;
}

Type* CodeGenUtils::getType(shared_ptr<TypeNode> node) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "boolean") {
            return IntegerType::get(*(codeGen->TheContext), 1);
        } else if (x->record->id == "int") {
            return IntegerType::get(*(codeGen->TheContext), 32);
        } else if (x->record->id == "byte") {
            return IntegerType::get(*(codeGen->TheContext), 8);
        } else if (x->record->id == "short") {
            return IntegerType::get(*(codeGen->TheContext), 16);
        } else if (x->record->id == "long") {
            return IntegerType::get(*(codeGen->TheContext), 64);
        } else if (x->record->id == "float") {
            return Type::getFloatTy(*(codeGen->TheContext));
        } else if (x->record->id == "double") {
            return Type::getDoubleTy(*(codeGen->TheContext));
        } else if (x->record->id == "char") {
            return IntegerType::get(*(codeGen->TheContext), 8);
        } else if (x->record->id == "void") {
            return Type::getVoidTy(*(codeGen->TheContext));
        }
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(getFullClassRecordName(x->record))) {
            structType = classesTypes.at(getFullClassRecordName(x->record));
        } else {
            Out::errorMessage("Can not get " + getFullClassRecordName(x->record));
        }
        PointerType *structPtrType = PointerType::get(structType, 0);
        return structPtrType;
    }
}

Type* CodeGenUtils::getTypeNoPtr(shared_ptr<TypeNode> node) {
    auto x = node->type;
    if (x->record->type == "primitive") {
        if (x->record->id == "boolean") {
            return IntegerType::get(*(codeGen->TheContext), 1);
        } else if (x->record->id == "int") {
            return IntegerType::get(*(codeGen->TheContext), 32);
        } else if (x->record->id == "byte") {
            return IntegerType::get(*(codeGen->TheContext), 8);
        } else if (x->record->id == "short") {
            return IntegerType::get(*(codeGen->TheContext), 16);
        } else if (x->record->id == "long") {
            return IntegerType::get(*(codeGen->TheContext), 64);
        } else if (x->record->id == "float") {
            return Type::getFloatTy(*(codeGen->TheContext));
        } else if (x->record->id == "double") {
            return Type::getDoubleTy(*(codeGen->TheContext));
        } else if (x->record->id == "char") {
            return IntegerType::get(*(codeGen->TheContext), 8);
        } else if (x->record->id == "void") {
            return Type::getVoidTy(*(codeGen->TheContext));
        }
    } else {
        StructType *structType = nullptr;
        if (classesTypes.contains(getFullClassRecordName(x->record))) {
            structType = classesTypes.at(getFullClassRecordName(x->record));
        } else {
            Out::errorMessage("Can not get " + getFullClassRecordName(x->record));
        }
        return structType;
    }
}