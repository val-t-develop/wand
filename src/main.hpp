#pragma once
#include <symbolTable/SymbolTable.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>
#include <lexer/Lexer.hpp>
#include <symbolListener/SymbolListener.hpp>

class Main {
public:
    static vector<string> obj_files;
    static string fullFileName;
    static string filename;

    static void main(int argc, char **argv);

    static void processDir(Path dir);

    static void processFile(Path file);

    static bool validSplFile(const string &name);

    static shared_ptr<SymbolTable> symbolTable;

    static void codeGeneration(Path &filePath);

    static shared_ptr<SymbolTable> getSymTab(Path path);
};
