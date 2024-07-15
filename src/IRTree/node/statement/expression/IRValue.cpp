//
// Created by val.t on 7/15/24.
//

#include "IRValue.hpp"

#include <utils/Out.hpp>

IRValue::IRValue(llvm::Value *_val) : val(_val), IRExpression(Kind::VALUE) {}

string IRValue::getReturnType(CodeGen* codeGen) {
    Out::errorMessage("Can not get type of value");
    return "";
}