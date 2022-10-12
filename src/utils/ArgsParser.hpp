#pragma once
#include <Defs.hpp>
#include <utils/Path.hpp>

class ArgsParser {
public:
    static Path output;
    static vector<Path> src;

    static void parseArgs(int argc, char **argv);
    static void parseArgs(vector<string> args);
};
