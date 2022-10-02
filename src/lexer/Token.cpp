#include "Token.hpp"

Token::Token(Kind kind, string str, int line, int pos) {
    this->str = str;
    this->kind = kind;
    this->line = line;
    this->pos = pos;
}
