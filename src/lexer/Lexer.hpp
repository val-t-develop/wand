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
 *  Lexer of code.
 */

#pragma once
#include "Token.hpp"
#include <Defs.hpp>
#include <utils/Path.hpp>

class Lexer {
  public:
    Path path;
    string src;
    vector<shared_ptr<Token>> tokens = vector<shared_ptr<Token>>();
    int currToken = 0, currChar = 0;
    int currLine = 0, currPos = 0;

    Lexer();
    Lexer(Path &filePath);
    Lexer(Lexer &r);

    void tokenize();
    void tokenizeIdentifier();
    void tokenizeDec();
    void tokenizeChar();
    void tokenizeString();

    void incCurrChar();

    shared_ptr<Token> getWithNumber(size_t n);
    shared_ptr<Token> getWithOffset(int offset);
    shared_ptr<Token> getCurrent();
    shared_ptr<Token> getNext();
    shared_ptr<Token> getPrevious();
    shared_ptr<Token> goWithOffset(int offset);
    shared_ptr<Token> goForward();
    shared_ptr<Token> goBack();

    void removeFirstCharOfCurrShift();
    bool ifCurrTokenStartsWithLT();
    bool ifCurrTokenStartsWithGT();

    void reset();
};
