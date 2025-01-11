/*  WAND - Wand Programming Language compiler
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Parser of compiler arguments.
 */

#include "ArgsParser.hpp"
#include "License.hpp"
#include "main.hpp"

#include <utils/Out.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

shared_ptr<Path> ArgsParser::output = nullptr;
vector<Path> ArgsParser::src = vector<Path>();
shared_ptr<Path> ArgsParser::emit_llvm = nullptr;

void ArgsParser::parseArgs(int argc, char **argv) {
    string home = getenv("HOME");
    if (home.size() != 1 && (home.ends_with('/') || home.ends_with('\\'))) {
        home.pop_back();
    }
    vector<string> vec;
    for (int i = 0; i < argc; ++i) {
        auto curr = wandit(string(argv[i]), " ");
        for (auto el : curr) {
            if (el.find("~") != string::npos) {
                el.replace(el.find("~"), string("~").length(), home);
            }
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
            output = make_shared<Path>(arg);
        } else if (arg=="-g" || arg=="-d") {
            Main::debug=true;
        } else if (arg=="-r") {
            Main::release=true;
        } else if (arg=="--emit-llvm" || arg=="-l") {
            i++;
            arg = args[i];
            emit_llvm = make_shared<Path>(arg);
        } else if (arg=="-o0") {
            Main::optLevel=Main::OptLevel::O0;
        } else if (arg=="-o1") {
            Main::optLevel=Main::OptLevel::O1;
        } else if (arg=="-o2") {
            Main::optLevel=Main::OptLevel::O2;
        } else if (arg=="-o3") {
            Main::optLevel=Main::OptLevel::O3;
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
    if (Main::debug) {
        Main::optLevel=Main::OptLevel::O0;
    }
    if (Main::release) {
        Main::optLevel=Main::OptLevel::O3;
    }
    if (Main::release && Main::debug) {
        Out::errorMessage("You can not use both debug flag (-d or -g) ang release flag (-r)");
    }
}
