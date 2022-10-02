#pragma once
#include <lexer/Lexer.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <utils/Path.hpp>
#include <ast/Ast.hpp>

class AstBuilder {
public:
    shared_ptr<SymbolTable> symbolTable;
    Path filePath;
    Lexer lexer;

    AstBuilder(shared_ptr<SymbolTable> _symbolTable, Path _filePath, Lexer _lexer);


};
