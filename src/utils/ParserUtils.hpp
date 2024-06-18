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
 *  Helper file for parser.
 */

#pragma once
#include <lexer/Lexer.hpp>

class ParserUtils {
  public:
    static map<string, int> operatorsPrecedence;
    static int getBinOpPrecedence(Lexer &lexer);

    static void skipSemicolons(Lexer &lexer);

    static bool isModifier(shared_ptr<Token> token);

    static long double parseDouble(string token);
    static int64_t parseLong(string token);

    class QualifiedName {
        vector<shared_ptr<Token>> list = vector<shared_ptr<Token>>();

      public:
        QualifiedName(Lexer &lexer);

        string getText();

        int size();

        void add(shared_ptr<Token> token);

        shared_ptr<Token> get(int index);

        // void add(int index, Token element) {
        // list.insert(std::next(list.begin(), index), element); }

        vector<shared_ptr<Token>> getList();

        void setList(vector<shared_ptr<Token>> list);

        vector<string> getTextList();
    };
};
