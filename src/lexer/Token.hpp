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
 *  Token that is used by lexer to represent the smallest code part.
 */

#pragma once
#include <Defs.hpp>

class Token {
  public:
    enum class Kind {
        PACKAGE,
        IMPORT,
        PUBLIC,
        PRIVATE,
        PROTECTED,
        STATIC,
        FINAL,
        VOLATILE,
        TRANSIENT,
        SYNCHRONIZED,
        ABSTRACT,
        NATIVE,
        STRICTFP,
        CONST,
        BOOLEAN,
        BYTE,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        CHAR,
        VOID,
        FOR,
        WHILE,
        DO,
        BREAK,
        CONTINUE,
        GOTO,
        IF,
        ELSE,
        SWITCH,
        CASE,
        DEFAULT,
        TRY,
        CATCH,
        FINALLY,
        THROW,
        THROWS,
        CLASS,
        INTERFACE,
        ENUM,
        EXTENDS,
        IMPLEMENTS,
        INSTANCEOF,
        THIS,
        SUPER,
        RETURN,
        NEW,
        ASSERT,

        DEC_INT_LITERAL,
        HEX_INT_LITERAL,
        DEC_LONG_LITERAL,
        HEX_LONG_LITERAL,
        DEC_FLOAT_LITERAL,
        HEX_FLOAT_LITERAL,
        DEC_DOUBLE_LITERAL,
        HEX_DOUBLE_LITERAL,
        BOOLEAN_LITERAL,
        CHAR_LITERAL,
        STRING_LITERAL,
        NULL_LITERAL,

        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        SEMICOLON,
        COLON,
        COMMA,
        DOT,
        ASSIGN,
        EQUAL,
        GT,
        LT,
        BANG,
        LT_EQ,
        GT_EQ,
        NOT_EQ,
        AND,
        OR,
        TILDE,
        QUESTION,
        INC,
        DEC,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        BIT_AND,
        BIT_OR,
        XOR,
        ARROW,
        COLON_COLON,
        LSHIFT,
        RSHIFT,
        URSHIFT,
        ADD_ASSIGN,
        SUB_ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        AND_ASSIGN,
        OR_ASSIGN,
        XOR_ASSIGN,
        LSHIFT_ASSIGN,
        RSHIFT_ASSIGN,
        URSHIFT_ASSIGN,
        AT,
        ELLIPSIS,

        IDENTIFIER,

        END_OF_FILE
    };

    string str;
    Kind kind;
    int line;
    int pos;

    Token(Kind kind, string str, int line, int pos);
};
