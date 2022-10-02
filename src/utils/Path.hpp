#pragma once
#include <Defs.hpp>

class Path {
    fs::path path;
    string content;
public:
    Path();
    Path(string name);
    Path(fs::path &path);

    string getName();
    string getFilename();
    string readFile();
    bool isFile();
    bool isDir();
    vector<Path> getDirContent();
    Path getParent();
    static Path getCurrentDir();
};
