//
// Created by val.t on 7/15/24.
//

#include "IRValue.hpp"

IRValue::IRValue(llvm::Value *_val) : val(_val), IRExpression(Kind::VALUE) {}