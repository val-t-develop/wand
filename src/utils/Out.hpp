#pragma once
#include <Defs.hpp>
#include <lexer/Lexer.hpp>

class Out {
  public:
    static int messages;

    static void errorMessage(string msg);
    static void printMessage(string msg);

    static void errorMessage(Lexer &lexer, string msg);
};
