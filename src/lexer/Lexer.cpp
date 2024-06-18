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
 *  Lexer of code.
 */

#include "Lexer.hpp"
#include <utils/Out.hpp>

Lexer::Lexer() {}

Lexer::Lexer(Path &filePath) {
    this->path = filePath;
    this->src = filePath.readFile();
}

Lexer::Lexer(Lexer &r) {
    this->path = r.path;
    this->src = r.src;
    this->currToken = r.currToken;
    this->currChar = r.currChar;
    this->currLine = r.currLine;
    this->currPos = r.currPos;
    for (shared_ptr<Token> tok : r.tokens) {
        tokens.push_back(
            make_shared<Token>(tok->kind, tok->str, tok->line, tok->pos));
    }
}

void Lexer::tokenize() {
    while (true) {
        char ch = '\0', ch1 = '\0', ch2 = '\0', ch3 = '\0';
        try {
            ch = src.at(currChar);
            ch1 = src.at(currChar + 1);
            ch2 = src.at(currChar + 2);
            ch3 = src.at(currChar + 3);
        } catch (std::out_of_range &ignored) {
        }

        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
            incCurrChar();
            continue;
        }

        if (ch == '/' && ch1 == '/') {
            while (src.at(currChar) != '\n') {
                incCurrChar();
            }
            continue;
        }

        if (ch == '/' && ch1 == '*') {
            while (true) {
                if (src.at(currChar) != '/' && src.at(currChar - 1) != '*') {
                    incCurrChar();
                    break;
                }
                incCurrChar();
            }
            continue;
        }

        if (ch == '\0') {
            break;
        } else if (isalpha(ch)) {
            tokenizeIdentifier();
        } else if (isdigit(ch)) {
            tokenizeDec();
        } else if (ch == '.' && isdigit(src.at(currChar + 1))) {
            tokenizeDec();
        } else if (ch == '\'') {
            tokenizeChar();
        } else if (ch == '"') {
            tokenizeString();
        }

        else if (ch == '(') {
            tokens.push_back(make_shared<Token>(Token::Kind::LPAREN, "(",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == ')') {
            tokens.push_back(make_shared<Token>(Token::Kind::RPAREN, ")",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '{') {
            tokens.push_back(make_shared<Token>(Token::Kind::LBRACE, "{",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '}') {
            tokens.push_back(make_shared<Token>(Token::Kind::RBRACE, "}",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '[') {
            tokens.push_back(make_shared<Token>(Token::Kind::LBRACKET, "[",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == ']') {
            tokens.push_back(make_shared<Token>(Token::Kind::RBRACKET, "]",
                                                currLine, currPos));
            incCurrChar();
        }

        else if (ch == ';') {
            tokens.push_back(make_shared<Token>(Token::Kind::SEMICOLON, ";",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == ',') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::COMMA, ",", currLine, currPos));
            incCurrChar();
        } else if (ch == '~') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::TILDE, "~", currLine, currPos));
            incCurrChar();
        } else if (ch == '?') {
            tokens.push_back(make_shared<Token>(Token::Kind::QUESTION, "?",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '@') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::AT, "@", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '<' && ch1 == '<' && ch2 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::LSHIFT_ASSIGN,
                                                "<<=", currLine, currPos));
            incCurrChar();
            incCurrChar();
            incCurrChar();
        } else if (ch == '>' && ch1 == '>' && ch2 == '>' && ch3 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::URSHIFT_ASSIGN,
                                                ">>>=", currLine, currPos));
            incCurrChar();
            incCurrChar();
            incCurrChar();
            incCurrChar();
        } else if (ch == '>' && ch1 == '>' && ch2 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::RSHIFT_ASSIGN,
                                                ">>=", currLine, currPos));
            incCurrChar();
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '<' && ch1 == '<') {
            tokens.push_back(make_shared<Token>(Token::Kind::LSHIFT, "<<",
                                                currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '>' && ch1 == '>' && ch2 == '>') {
            tokens.push_back(make_shared<Token>(Token::Kind::URSHIFT, ">>>",
                                                currLine, currPos));
            incCurrChar();
            incCurrChar();
            incCurrChar();
        } else if (ch == '>' && ch1 == '>') {
            tokens.push_back(make_shared<Token>(Token::Kind::RSHIFT, ">>",
                                                currLine, currPos));
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '<' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::LT_EQ,
                                                "<=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '>' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::GT_EQ,
                                                ">=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '<') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::LT, "<", currLine, currPos));
            incCurrChar();
        } else if (ch == '>') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::GT, ">", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '=' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::EQUAL,
                                                "==", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::ASSIGN, "=",
                                                currLine, currPos));
            incCurrChar();
        }

        else if (ch == '+' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::ADD_ASSIGN,
                                                "+=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '-' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::SUB_ASSIGN,
                                                "-=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '*' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::MUL_ASSIGN,
                                                "*=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '/' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::DIV_ASSIGN,
                                                "/=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '%' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::MOD_ASSIGN,
                                                "%=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '&' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::AND_ASSIGN,
                                                "&=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '|' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::OR_ASSIGN,
                                                "|=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '^' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::XOR_ASSIGN,
                                                "^=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '!' && ch1 == '=') {
            tokens.push_back(make_shared<Token>(Token::Kind::NOT_EQ,
                                                "!=", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '!') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::BANG, "!", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '&' && ch1 == '&') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::AND, "&&", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '|' && ch1 == '|') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::OR, "||", currLine, currPos));
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '+' && ch1 == '+') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::INC, "++", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == '-' && ch1 == '-') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::DEC, "--", currLine, currPos));
            incCurrChar();
            incCurrChar();
        }

        else if (ch == '&') {
            tokens.push_back(make_shared<Token>(Token::Kind::BIT_AND, "&",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '|') {
            tokens.push_back(make_shared<Token>(Token::Kind::BIT_OR, "|",
                                                currLine, currPos));
            incCurrChar();
        } else if (ch == '^') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::XOR, "^", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '-' && ch1 == '>') {
            tokens.push_back(make_shared<Token>(Token::Kind::ARROW, "->",
                                                currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == ':' && ch1 == ':') {
            tokens.push_back(make_shared<Token>(Token::Kind::COLON_COLON,
                                                "::", currLine, currPos));
            incCurrChar();
            incCurrChar();
        } else if (ch == ':') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::COLON, ":", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '+') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::ADD, "+", currLine, currPos));
            incCurrChar();
        } else if (ch == '-') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::SUB, "-", currLine, currPos));
            incCurrChar();
        } else if (ch == '*') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::MUL, "*", currLine, currPos));
            incCurrChar();
        } else if (ch == '/') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::DIV, "/", currLine, currPos));
            incCurrChar();
        } else if (ch == '%') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::MOD, "%", currLine, currPos));
            incCurrChar();
        }

        else if (ch == '.' && ch1 == '.' && ch2 == '.') {
            tokens.push_back(make_shared<Token>(Token::Kind::ELLIPSIS, "...",
                                                currLine, currPos));
            incCurrChar();
            incCurrChar();
            incCurrChar();
        } else if (ch == '.') {
            tokens.push_back(
                make_shared<Token>(Token::Kind::DOT, ".", currLine, currPos));
            incCurrChar();
        }

        else {
            Out::errorMessage("Can not tokenize this:\n" +
                              src.substr(currChar, currChar + 20));
        }
    }
}

void Lexer::tokenizeIdentifier() {
    string str = "";
    Token::Kind kind = Token::Kind::IDENTIFIER;
    int line = currLine;
    int pos = currPos;

    while (true) {
        char ch = src.at(currChar);
        if (!isalpha(ch) && !isdigit(ch)) {
            break;
        }
        str.push_back(ch);
        incCurrChar();
    }

    if (str == "package") {
        kind = Token::Kind::PACKAGE;
    } else if (str == "import") {
        kind = Token::Kind::IMPORT;
    } else if (str == "public") {
        kind = Token::Kind::PUBLIC;
    } else if (str == "private") {
        kind = Token::Kind::PRIVATE;
    } else if (str == "protected") {
        kind = Token::Kind::PROTECTED;
    } else if (str == "static") {
        kind = Token::Kind::STATIC;
    } else if (str == "final") {
        kind = Token::Kind::FINAL;
    } else if (str == "volatile") {
        kind = Token::Kind::VOLATILE;
    } else if (str == "transient") {
        kind = Token::Kind::TRANSIENT;
    } else if (str == "synchronized") {
        kind = Token::Kind::SYNCHRONIZED;
    } else if (str == "abstract") {
        kind = Token::Kind::ABSTRACT;
    } else if (str == "native") {
        kind = Token::Kind::NATIVE;
    } else if (str == "strictfp") {
        kind = Token::Kind::STRICTFP;
    } else if (str == "const") {
        kind = Token::Kind::CONST;
    } else if (str == "boolean") {
        kind = Token::Kind::BOOLEAN;
    } else if (str == "byte") {
        kind = Token::Kind::BYTE;
    } else if (str == "short") {
        kind = Token::Kind::SHORT;
    } else if (str == "int") {
        kind = Token::Kind::INT;
    } else if (str == "long") {
        kind = Token::Kind::LONG;
    } else if (str == "float") {
        kind = Token::Kind::FLOAT;
    } else if (str == "double") {
        kind = Token::Kind::DOUBLE;
    } else if (str == "char") {
        kind = Token::Kind::CHAR;
    } else if (str == "void") {
        kind = Token::Kind::VOID;
    } else if (str == "for") {
        kind = Token::Kind::FOR;
    } else if (str == "while") {
        kind = Token::Kind::WHILE;
    } else if (str == "do") {
        kind = Token::Kind::DO;
    } else if (str == "break") {
        kind = Token::Kind::BREAK;
    } else if (str == "continue") {
        kind = Token::Kind::CONTINUE;
    } else if (str == "goto") {
        kind = Token::Kind::GOTO;
    } else if (str == "if") {
        kind = Token::Kind::IF;
    } else if (str == "else") {
        kind = Token::Kind::ELSE;
    } else if (str == "switch") {
        kind = Token::Kind::SWITCH;
    } else if (str == "case") {
        kind = Token::Kind::CASE;
    } else if (str == "default") {
        kind = Token::Kind::DEFAULT;
    } else if (str == "try") {
        kind = Token::Kind::TRY;
    } else if (str == "catch") {
        kind = Token::Kind::CATCH;
    } else if (str == "finally") {
        kind = Token::Kind::FINALLY;
    } else if (str == "throw") {
        kind = Token::Kind::THROW;
    } else if (str == "throws") {
        kind = Token::Kind::THROWS;
    } else if (str == "class") {
        kind = Token::Kind::CLASS;
    } else if (str == "interface") {
        kind = Token::Kind::INTERFACE;
    } else if (str == "enum") {
        kind = Token::Kind::ENUM;
    } else if (str == "extends") {
        kind = Token::Kind::EXTENDS;
    } else if (str == "implements") {
        kind = Token::Kind::IMPLEMENTS;
    } else if (str == "instanceof") {
        kind = Token::Kind::INSTANCEOF;
    } else if (str == "this") {
        kind = Token::Kind::THIS;
    } else if (str == "super") {
        kind = Token::Kind::SUPER;
    } else if (str == "return") {
        kind = Token::Kind::RETURN;
    } else if (str == "new") {
        kind = Token::Kind::NEW;
    } else if (str == "assert") {
        kind = Token::Kind::ASSERT;
    } else if (str == "true" || str == "false") {
        kind = Token::Kind::BOOLEAN_LITERAL;
    } else if (str == "null") {
        kind = Token::Kind::NULL_LITERAL;
    }
    tokens.push_back(make_shared<Token>(kind, str, line, pos));
}

void Lexer::tokenizeDec() {
    string str = "";
    Token::Kind kind = Token::Kind::DEC_INT_LITERAL;
    int line = currLine;
    int pos = currPos;

    while (true) {
        char ch = src.at(currChar);
        if (isdigit(ch)) {
            str.push_back(ch);
            incCurrChar();
            continue;
        }
        if (ch == '_') {
            incCurrChar();
            continue;
        }
        if (ch == '.') {
            kind = Token::Kind::DEC_FLOAT_LITERAL;
            str.push_back(ch);
            incCurrChar();
            continue;
        }
        break;
    }

    char ch = src.at(currChar);

    if (ch == 'l' || ch == 'L') {
        kind = Token::Kind::DEC_LONG_LITERAL;
    } else if (ch == 'f' || ch == 'F') {
        kind = Token::Kind::DEC_FLOAT_LITERAL;
    } else if (ch == 'd' || ch == 'D') {
        kind = Token::Kind::DEC_DOUBLE_LITERAL;
    }

    tokens.push_back(make_shared<Token>(kind, str, line, pos));
}

void Lexer::tokenizeChar() {
    string str = "";
    int line = currLine;
    int pos = currPos;

    incCurrChar();

    while (true) {
        char ch = src.at(currChar);
        if (ch == '\'' && src.at(currChar - 1) != '\\') {
            incCurrChar();
            break;
        } else {
            str.push_back(ch);
            incCurrChar();
        }
    }

    tokens.push_back(
        make_shared<Token>(Token::Kind::CHAR_LITERAL, str, line, pos));
}

void Lexer::tokenizeString() {
    string str = "";
    int line = currLine;
    int pos = currPos;

    incCurrChar();

    while (true) {
        char ch = src.at(currChar);
        if (ch == '"' && src.at(currChar - 1) != '\\') {
            incCurrChar();
            break;
        } else {
            str.push_back(ch);
            incCurrChar();
        }
    }

    tokens.push_back(
        make_shared<Token>(Token::Kind::STRING_LITERAL, str, line, pos));
}

void Lexer::incCurrChar() {
    if (src.at(currChar) == '\n') {
        currLine++;
        currPos = 1;
    } else {
        currPos++;
    }
    currChar++;
}

shared_ptr<Token> Lexer::getWithNumber(size_t n) {
    if (tokens.size() <= n) {
        return make_shared<Token>(Token::Kind::END_OF_FILE, "\0", -1, -1);
    } else {
        return tokens[n];
    }
}

shared_ptr<Token> Lexer::getWithOffset(int offset) {
    return getWithNumber(currToken + offset);
}

shared_ptr<Token> Lexer::getCurrent() { return getWithOffset(0); }

shared_ptr<Token> Lexer::getNext() { return getWithOffset(1); }

shared_ptr<Token> Lexer::getPrevious() { return getWithOffset(-1); }

shared_ptr<Token> Lexer::goWithOffset(int offset) {
    currToken += offset;
    return getCurrent();
}

shared_ptr<Token> Lexer::goForward() { return goWithOffset(1); }

shared_ptr<Token> Lexer::goBack() { return goWithOffset(-1); }

void Lexer::removeFirstCharOfCurrShift() {
    if (getCurrent()->kind == Token::Kind::LT) {
        goForward();
    } else if (getCurrent()->kind == Token::Kind::GT) {
        goForward();
    } else if (getCurrent()->kind == Token::Kind::LT_EQ) {
        getCurrent()->kind = Token::Kind::ASSIGN;
        getCurrent()->str = "=";
    } else if (getCurrent()->kind == Token::Kind::GT_EQ) {
        getCurrent()->kind = Token::Kind::ASSIGN;
        getCurrent()->str = "=";
    } else if (getCurrent()->kind == Token::Kind::LSHIFT) {
        getCurrent()->kind = Token::Kind::LT;
        getCurrent()->str = "<";
    } else if (getCurrent()->kind == Token::Kind::RSHIFT) {
        getCurrent()->kind = Token::Kind::GT;
        getCurrent()->str = ">";
    } else if (getCurrent()->kind == Token::Kind::LSHIFT_ASSIGN) {
        getCurrent()->kind = Token::Kind::LT_EQ;
        getCurrent()->str = "<=";
    } else if (getCurrent()->kind == Token::Kind::RSHIFT_ASSIGN) {
        getCurrent()->kind = Token::Kind::GT_EQ;
        getCurrent()->str = ">=";
    } else if (getCurrent()->kind == Token::Kind::URSHIFT) {
        getCurrent()->kind = Token::Kind::RSHIFT;
        getCurrent()->str = ">>";
    } else if (getCurrent()->kind == Token::Kind::URSHIFT_ASSIGN) {
        getCurrent()->kind = Token::Kind::RSHIFT_ASSIGN;
        getCurrent()->str = ">>=";
    }
}

bool Lexer::ifCurrTokenStartsWithLT() {
    return getCurrent()->kind == Token::Kind::LT ||
           getCurrent()->kind == Token::Kind::LT_EQ ||
           getCurrent()->kind == Token::Kind::LSHIFT ||
           getCurrent()->kind == Token::Kind::LSHIFT_ASSIGN;
}

bool Lexer::ifCurrTokenStartsWithGT() {
    return getCurrent()->kind == Token::Kind::GT ||
           getCurrent()->kind == Token::Kind::GT_EQ ||
           getCurrent()->kind == Token::Kind::RSHIFT ||
           getCurrent()->kind == Token::Kind::RSHIFT_ASSIGN ||
           getCurrent()->kind == Token::Kind::URSHIFT ||
           getCurrent()->kind == Token::Kind::URSHIFT_ASSIGN;
}

void Lexer::reset() { currToken = 0; }
