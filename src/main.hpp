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

#pragma once
#include <IRTree/builder/IRTreeBuilder.hpp>
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/CodeGen.hpp>
#include <lexer/Lexer.hpp>
#include <symbolListener/SymbolListener.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>

class CU {
  public:
    enum State { NOTHING = 0, LEXER = 1, ST = 2, AST = 3, IR_TREE = 4, CODE_GEN = 5 };

    string fullFileName;
    string fileName;
    Path file;

    Lexer lexer;
    shared_ptr<SymbolTable> st;
    shared_ptr<SymbolListener> symbolListener;
    shared_ptr<CompilationUnitNode> cu;
    shared_ptr<AstBuilder> astBuilder;
    shared_ptr<IRTreeBuilder> irTreeBuilder;
    shared_ptr<CodeGen> codeGen;

    map<vector<string>, vector<Path>> importFiles =
        map<vector<string>, vector<Path>>();
    vector<string> linkingObj = vector<string>();

    State currState = State::NOTHING;

    CU(string _fullFileName, string _fileName, Path _file);
    void completeToState(State state);
};

class Main {
  public:
    static vector<string> obj_files;
    static stack<CU *> currCUsStack;
    static map<Path, shared_ptr<CU>> CUs;

    static void main(int argc, char **argv);
    static void processDir(Path &dir);
    static void processFile(Path &file);
    static bool validSplFile(const string &name);
    static void processFileToState(Path &file, CU::State state);
};
