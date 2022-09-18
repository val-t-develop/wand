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
    string readFile();
    bool isFile();
    bool isDir();
    vector<Path> getDirContent();
    static Path getCurrentDir();
};