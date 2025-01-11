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
 *  IR tree node.
 */

#include "IRNode.hpp"

IRNode::IRNode(Kind _kind, int _line, int _col) : kind(_kind), line(_line), col(_col) {}

bool IRNode::isExpression() {
    if (kind==Kind::VALUE ||
        kind==Kind::VAR ||
        kind==Kind::ACCESS ||
        kind == Kind::CALL ||
        kind == Kind::UN_OP ||
        kind == Kind::BIN_OP ||
        kind == Kind::ALLOC ||
        kind == Kind::FUNCTION_POINTER ||
        isLiteral()) {
        return true;
    } else {
        return false;
    }
}

bool IRNode::isLiteral() {
    if (kind == Kind::STRING_LITERAL || kind == Kind::CHAR_LITERAL ||
        kind == Kind::BOOL_LITERAL || kind == Kind::BYTE_LITERAL ||
        kind == Kind::SHORT_LITERAL || kind == Kind::INT_LITERAL ||
        kind == Kind::LONG_LITERAL || kind == Kind::FLOAT_LITERAL ||
        kind == Kind::DOUBLE_LITERAL || kind == Kind::NULL_LITERAL) {
        return true;
    } else {
        return false;
    }
}

void IRNode::setPos(int _line, int _col) {
    line=_line;
    col=_col;
}