#include "ArgsParser.hpp"

#include <utils/Out.hpp>


Path ArgsParser::outputDir = Path();
vector<Path> ArgsParser::src = vector<Path>();

void ArgsParser::parseArgs(int argc, char **argv) {
    vector<string> vec;
    for(int i = 0; i < argc; ++i) {
        auto curr = split(string(argv[i]), " ");
        for(auto el : curr) {
            vec.push_back(el);
        }
    }
    parseArgs(vec);
}

void ArgsParser::parseArgs(vector<string> args) {
    for(size_t i = 1; i < args.size(); i++) {
        string arg = args[i];
        if(arg.ends_with("-o")) {
            i++;
            arg = args[i];
            outputDir = Path(arg);
        } else {
            src.push_back(Path(arg));
        }
    }
    if(src.empty()) {
        Out::printMessage("No input files.");
        exit(0);
    }
}
