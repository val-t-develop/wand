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
 *  IR tree node.
 */

#pragma once
#include <Defs.hpp>

class IRNode {
public:
    enum class Kind {
        NONE,
        TREE,
        FUNCTION,
        STRUCT,
        BLOCK,
        IF_ELSE,
        RETURN,
        VAR_DECL,
        WHILE,

        VAR,
        ACCESS,
        CALL,
        BIN_OP,
        UN_OP,
        ALLOC,
        FUNCTION_POINTER,

        STRING_LITERAL,
        CHAR_LITERAL,
        BOOL_LITERAL,
        BYTE_LITERAL,
        SHORT_LITERAL,
        INT_LITERAL,
        LONG_LITERAL,
        FLOAT_LITERAL,
        DOUBLE_LITERAL,
        NULL_LITERAL
    };

    Kind kind = Kind::NONE;

    IRNode(Kind _kind);

    bool isExpression();
    bool isLiteral();
};
