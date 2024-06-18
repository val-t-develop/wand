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
 *  Parser of compiler arguments.
 */

#include "ArgsParser.hpp"
#include "License.hpp"

#include <utils/Out.hpp>

Path ArgsParser::output = Path();
vector<Path> ArgsParser::src = vector<Path>();

void ArgsParser::parseArgs(int argc, char **argv) {
    vector<string> vec;
    for (int i = 0; i < argc; ++i) {
        auto curr = split(string(argv[i]), " ");
        for (auto el : curr) {
            vec.push_back(el);
        }
    }
    parseArgs(vec);
}

void ArgsParser::parseArgs(vector<string> args) {
    for (size_t i = 1; i < args.size(); i++) {
        string arg = args[i];
        if (arg=="-o") {
            i++;
            arg = args[i];
            output = Path(arg);
        } else if (arg=="show") {
            i++;
            arg = args[i];
            if (arg=="c" || arg=="w") {
                Out::printMessage(LICENSE);
            }
        } else {
            src.push_back(Path(arg));
        }
    }
    if (src.empty()) {
        Out::printMessage("No input files.");
        exit(0);
    }
}
