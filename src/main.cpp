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
 *  Main project file.
 */

#include "main.hpp"
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/CodeGen.hpp>

vector<string> Main::obj_files = vector<string>();
stack<CU *> Main::currCUsStack = stack<CU *>();
map<Path, shared_ptr<CU>> Main::CUs = map<Path, shared_ptr<CU>>();

void Main::main(int argc, char **argv) {
    ArgsParser::parseArgs(argc, argv);
    for (Path file : ArgsParser::src) {
        if (file.isFile()) {
            processFile(file);
        } else if (file.isDir()) {
            processDir(file);
        }
    }

    for (auto currCU : CUs) {
        for (auto obj : currCU.second->linkingObj) {
            bool found = false;
            for (auto existing : obj_files) {
                if (existing == obj) {
                    found = true;
                }
            }
            if (found == false) {
                obj_files.push_back(obj);
            }
        }
    }

    string ld = "clang -o " + ArgsParser::output.getName();
    for (string obj : obj_files) {
        ld += " " + obj;
    }
    system(ld.c_str());

    string rm = "rm ";
    for (string obj : obj_files) {
        rm += " " + obj;
    }
    system(rm.c_str());
}

void Main::processDir(Path &dir) {
    for (Path file : dir.getDirContent()) {
        if (file.isFile()) {
            processFile(file);
        } else if (file.isDir()) {
            processDir(file);
        }
    }
}

void Main::processFile(Path &file) {
    Out::printMessage("Processing file: " + file.getName());
    processFileToState(file, CU::State::CODE_GEN);
}

void Main::processFileToState(Path &file, CU::State state) {
    if (CUs.contains(file)) {
        CUs[file]->completeToState(CU::State::CODE_GEN);
    } else {
        string fullName = file.getName();
        if (!validSplFile(fullName)) {
            Out::printMessage("Only generate .spl files valid");
            return;
        }
        string name = fullName.substr(0, fullName.find("."));

        shared_ptr<CU> cu = make_shared<CU>(fullName, name, file);
        cu->completeToState(state);
        CUs[file] = cu;
    }
}

bool Main::validSplFile(const string &name) { return name.ends_with(".spl"); }

CU::CU(string _fullFileName, string _fileName, Path _file)
    : fullFileName(_fullFileName), fileName(_fileName), file(_file) {}

void CU::completeToState(State state) {
    while (currState < state) {
        Main::currCUsStack.push(this);
        if (currState == State::NOTHING) {
            lexer = Lexer(file);
            lexer.tokenize();
            currState = State::LEXER;
        } else if (currState == State::LEXER) {
            st = make_shared<SymbolTable>();
            symbolListener = make_shared<SymbolListener>(st, file, lexer);
            symbolListener->walk();
            st->resetTable();
            currState = State::ST;
        } else if (currState == State::ST) {
            astBuilder = make_shared<AstBuilder>(st, file, lexer);
            cu = astBuilder->walk();
            st->resetTable();
            currState = State::AST;
        } else if (currState == State::AST) {
            codeGen = make_shared<CodeGen>(cu);
            codeGen->codeGen();
            codeGen->build();
            currState = State::CODE_GEN;
        } else if (currState == State::CODE_GEN) {
        }
        Main::currCUsStack.pop();
    }
}

int main(int argc, char **argv) {
    Main::main(argc, argv);
    return 0;
}
