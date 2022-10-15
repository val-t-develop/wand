#include "main.hpp"
#include <ast/builder/AstBuilder.hpp>
#include <codeGen/CodeGen.hpp>

vector<string> Main::obj_files = vector<string>();
stack<CU *> Main::currCUsStack = stack<CU *>();
map<Path, shared_ptr<CU>> Main::CUs = map<Path, shared_ptr<CU>>();

void Main::main(int argc, char **argv) {
  ArgsParser::parseArgs(argc, argv);
  for (Path file : ArgsParser::src) {
    if (file.isFile()) {
      processFile(file);
    } else if (file.isDir()) {
      processDir(file);
    }
  }
  string ld = "clang -o " + ArgsParser::output.getName();
  for (string obj : obj_files) {
    ld += " " + obj;
  }
  system(ld.c_str());
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
  Out::printMessage("Processing file: " + file.getName());
  processFileToState(file, CU::State::CODE_GEN);
}

void Main::processFileToState(Path &file, CU::State state) {
  if (CUs.contains(file)) {
    CUs[file]->completeToState(CU::State::CODE_GEN);
  } else {
    string fullName = file.getName();
    if (!validSplFile(fullName)) {
      Out::printMessage("Only generate .spl files valid");
      return;
    }
    string name = fullName.substr(0, fullName.find("."));
    
    shared_ptr<CU> cu = make_shared<CU>(fullName, name, file);
    cu->completeToState(state);
    CUs[file] = cu;
  }
}

bool Main::validSplFile(const string &name) { return name.ends_with(".spl"); }

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
      symbolListener = make_shared<SymbolListener>(st, file, lexer);
      symbolListener->walk();
      st->resetTable();
      currState = State::ST;
    } else if (currState == State::ST) {
      astBuilder = make_shared<AstBuilder>(st, file, lexer);
      cu = astBuilder->walk();
      st->resetTable();
      currState = State::AST;
    } else if (currState == State::AST) {
      codeGen = make_shared<CodeGen>(cu);
      codeGen->codeGen();
      codeGen->build();
      currState = State::CODE_GEN;
    } else if (currState == State::CODE_GEN) {
    }
    Main::currCUsStack.pop();
  }
}

int main(int argc, char **argv) {
  Main::main(argc, argv);
  return 0;
}
