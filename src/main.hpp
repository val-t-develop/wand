#pragma once
#include <symbolTable/SymbolTable.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>
#include <lexer/Lexer.hpp>
#include <symbolListener/SymbolListener.hpp>
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/CodeGen.hpp>

class CU {
public:
    enum State {
        NOTHING = 0, LEXER = 1, ST = 2, AST = 3, CODE_GEN = 4
    };

    string fullFileName;
    string fileName;
    Path file;

    Lexer lexer;
    shared_ptr<SymbolTable> st;
    shared_ptr<SymbolListener> symbolListener;
    shared_ptr<CompilationUnitNode> cu;
    shared_ptr<AstBuilder> astBuilder;
    shared_ptr<CodeGen> codeGen;

    map<vector<string>, vector<Path>> importFiles = map<vector<string>, vector<Path>>();

    State currState = State::NOTHING;

    CU(string _fullFileName, string _fileName, Path _file);
    void completeToState(State state);
};


class Main {
public:
    static vector<string> obj_files;
    static stack<CU*> currCUsStack;
    static map<Path, shared_ptr<CU>> CUs;

    static void main(int argc, char **argv);
    static void processDir(Path &dir);
    static void processFile(Path &file);
    static bool validSplFile(const string &name);
    static void processFileToState(Path &file, CU::State state);
};

