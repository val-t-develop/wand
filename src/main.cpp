#include "main.hpp"
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/CodeGen.hpp>

void Main::main(int argc, char **argv) {
    ArgsParser::parseArgs(argc, argv);
    for(Path file : ArgsParser::src) {
        if(file.isFile()) {
            processFile(file);
        } else if(file.isDir()) {
            processDir(file);
        }
    }
    string ld = "clang -o " + ArgsParser::output.getFilename();
    for (string obj : obj_files) {
        ld += " " + obj;
    }
    system(ld.c_str());
}

void Main::processDir(Path dir) {
    for(Path file : dir.getDirContent()) {
        if(file.isFile()) {
            processFile(file);
        } else if(file.isDir()) {
            processDir(file);
        }
    }
}

void Main::processFile(Path file) {
    fullFileName = file.getName();
    if(!validSplFile(fullFileName)) {
        Out::printMessage("Only generate .spl files valid");
        return;
    }
    filename = fullFileName.substr(0, fullFileName.find("."));
    Out::printMessage("Processing file: " + fullFileName);
    codeGeneration(file);
}

bool Main::validSplFile(const string &name) {
    return name.ends_with(".spl");
}

void Main::codeGeneration(Path &filePath) {
    Lexer lexer(filePath);
    lexer.tokenize();

    SymbolListener symbolListener = SymbolListener(symbolTable, filePath, lexer);
    symbolListener.walk();

    symbolTable->resetTable();

    AstBuilder astBuilder = AstBuilder(symbolTable, filePath, lexer);
    shared_ptr<CompilationUnitNode> cu = astBuilder.walk();

    symbolTable->resetTable();
    
    CodeGen codeGen(cu);
    codeGen.codeGen();
    codeGen.build();
}

shared_ptr<SymbolTable> Main::getSymTab(Path path) {
    shared_ptr<SymbolTable> symTab = make_shared<SymbolTable>();
    Lexer lexer = Lexer(path);
    lexer.tokenize();

    shared_ptr<SymbolListener> symbolListener =
        make_shared<SymbolListener> (symTab, path, lexer);
    symbolListener->walk();
    symbolTable->resetTable();
    return symTab;
}

string Main::fullFileName = "";
string Main::filename = "";
shared_ptr<SymbolTable> Main::symbolTable = make_shared<SymbolTable>();
vector<string> Main::obj_files = vector<string>();

int main(int argc, char **argv) {
    Main::main(argc, argv);
    return 0;
}
