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
 *  Output utility.
 */

#include "Out.hpp"

int Out::messages = 0;

void Out::errorMessage(string msg) {
    if (messages <= 50) {
        cerr << msg << "\n";
        messages++;
    } else {
        cerr << "Too many errors! Exit!\n";
        exit(-1);
    }
}

void Out::printMessage(string msg) { cout << msg << "\n"; }

void Out::errorMessage(Lexer &lexer, string msg) {
    if (messages <= 50) {
        cerr << lexer.path.getName() << ": " << msg << "\n";
        messages++;
    } else {
        cerr << "Too many errors! Exit!\n";
        exit(-1);
    }
}
