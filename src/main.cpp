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
 *  Main project file.
 */

#include "main.hpp"
#include <ast/builder/AstBuilder.hpp>

vector<string> Main::obj_files = vector<string>();
stack<CU *> Main::currCUsStack = stack<CU *>();
map<Path, shared_ptr<CU>> Main::CUs = map<Path, shared_ptr<CU>>();
bool Main::debug = false;
bool Main::release = false;
Main::OptLevel Main::optLevel = Main::OptLevel::O1;

void Main::main(int argc, char **argv) {
    ArgsParser::parseArgs(argc, argv);
    ArgsParser::src.push_back(Path("/usr/include/wand"));
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

    if (ArgsParser::output==nullptr) {
        ArgsParser::output=make_shared<Path>(Path::getCurrentDir().getName()+"/a.out");
    }
    string ld = "clang -o " + ArgsParser::output->getName();
    for (string obj : obj_files) {
        ld += " " + obj;
    }
    system(ld.c_str());

    string rm = "rm ";
    for (string obj : obj_files) {
        rm += " " + obj;
    }
    system(rm.c_str());
    system("rm -rd .wand_compilation");
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
    //Out::printMessage("Processing file: " + file.getName());
    processFileToState(file, CU::State::CODE_GEN);
}

void Main::processFileToState(Path &file, CU::State state) {
    if (CUs.contains(file)) {
        CUs[file]->completeToState(state);
    } else {
        string fullName = file.getName();
        if (!validWandFile(fullName)) {
            //Out::printMessage("Only generate .wand files valid");
            return;
        }
        string name = fullName.substr(0, fullName.find("."));

        shared_ptr<CU> cu = make_shared<CU>(fullName, name, file);
        CUs[file] = cu;
        cu->completeToState(state);
    }
}

bool Main::validWandFile(const string &name) { return name.ends_with(".wand"); }

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
            currState = State::ST;
            symbolListener = make_shared<SymbolListener>(st, file, lexer);
            symbolListener->walk();
            st->resetTable();
        } else if (currState == State::ST) {
            astBuilder = make_shared<AstBuilder>(st, file, lexer);
            cu = astBuilder->walk();
            st->resetTable();
            currState = State::AST;
        } else if (currState == State::AST) {
            irTreeBuilder=make_shared<IRTreeBuilder>(cu, file);
            irTreeBuilder->walk();
            currState = State::IR_TREE;
        } else if (currState == State::IR_TREE) {
            codeGen = make_shared<CodeGen>(irTreeBuilder->tree, file);
            codeGen->codeGen();
            codeGen->build();

            currState = State::CODE_GEN;
        } else if (currState == State::CODE_GEN) {}
        Main::currCUsStack.pop();
    }
}

int main(int argc, char **argv) {
    Main::main(argc, argv);
    return 0;
}
