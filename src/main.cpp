#include <symbolTable/SymbolTable.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>
#include <lexer/Lexer.hpp>

class Main {
public:
  static string fullFileName;
  static string filename;

  static void main(int argc, char **argv) {
    ArgsParser::parseArgs(argc, argv);
    for (Path file : ArgsParser::src) {
      if (file.isFile()) {
        processFile(file);
      } else if (file.isDir()) {
        processDir(file);
      }
    }
  }

  static void processDir(Path dir) {
    for (Path file : dir.getDirContent()) {
      if (file.isFile()) {
        processFile(file);
      } else if (file.isDir()) {
        processDir(file);
      }
    }
  }

  static void processFile(Path file) {
    fullFileName = file.getName();
    if (!validSplFile(fullFileName)) {
      Out::printMessage("Only generate .java files valid");
      return;
    }
    filename = fullFileName.substr(0, fullFileName.find("."));
    Out::printMessage("Processing file: " + fullFileName);
    codeGeneration(file);
  }

  static bool validSplFile(const string &name) {
    return name.ends_with(".spl");
  }

  static SymbolTable symbolTable;

  static void codeGeneration(Path& filePath) { 
    symbolTable = SymbolTable(); 
    Lexer lexer(filePath);
    lexer.tokenize();  
  }
};

string Main::fullFileName = "";
string Main::filename = "";
SymbolTable Main::symbolTable;

int main(int argc, char **argv) {
  Main::main(argc, argv);
  return 0;
}