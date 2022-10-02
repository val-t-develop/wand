#pragma once
#include <Defs.hpp>
#include <utils/Path.hpp>
#include "Token.hpp"

class Lexer {
public:
    Path path;
    string src;
    vector<shared_ptr<Token>> tokens = vector<shared_ptr<Token>>();
    int currToken = 0, currChar = 0;
    int currLine = 0, currPos = 0;

    Lexer(Path &filePath);
    Lexer(Lexer &r);

    void tokenize();
    void tokenizeIdentifier();
    void tokenizeDec();
    void tokenizeChar();
    void tokenizeString();

    void incCurrChar();

    shared_ptr<Token> getWithNumber(size_t n);
    shared_ptr<Token> getWithOffset(int offset);
    shared_ptr<Token> getCurrent();
    shared_ptr<Token> getNext();
    shared_ptr<Token> getPrevious();
    shared_ptr<Token> goWithOffset(int offset);
    shared_ptr<Token> goForward();
    shared_ptr<Token> goBack();

    void removeFirstCharOfCurrShift();
    bool ifCurrTokenStartsWithLT();
    bool ifCurrTokenStartsWithGT();

    void reset();
};
