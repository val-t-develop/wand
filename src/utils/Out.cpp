#include "Out.hpp"

int Out::messages = 0;

void Out::errorMessage(string msg) {
  if (messages <= 50) {
    cerr << "\n" << msg;
    messages++;
  } else {
    cerr << "\n Too many errors! Exit!";
    exit(-1);
  }
}

void Out::printMessage(string msg) { cout << "\n" << msg; }

void Out::errorMessage(Lexer& lexer, string msg) {
  if (messages <= 50) {
    cerr << "\n" << lexer.path.getName() << ": " << msg;
    messages++;
  } else {
    cerr << "\n Too many errors! Exit!";
    exit(-1);
  }
}