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
 *  IR tree part for literal.
 */

#include "IRLiteral.hpp"

IRLiteral::IRLiteral(Kind _kind) : IRExpression(_kind) {}

string IRLiteral::getReturnType(shared_ptr<CodeGen> codeGen) {
    if (kind==Kind::STRING_LITERAL) {
        return "String";
    } else if (kind==Kind::CHAR_LITERAL) {
        return "char";
    } else if (kind==Kind::BOOL_LITERAL) {
        return "bool";
    } else if (kind==Kind::BYTE_LITERAL) {
        return "byte";
    } else if (kind==Kind::SHORT_LITERAL) {
        return "short";
    } else if (kind==Kind::INT_LITERAL) {
        return "int";
    } else if (kind==Kind::LONG_LITERAL) {
        return "long";
    } else if (kind==Kind::FLOAT_LITERAL) {
        return "float";
    } else if (kind==Kind::DOUBLE_LITERAL) {
        return "double";
    } else if (kind==Kind::NULL_LITERAL) {
        return "null";
    }
}