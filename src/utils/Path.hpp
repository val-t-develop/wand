#pragma once
#include <Defs.hpp>
#include <compare>

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
    auto operator<=>(const Path&) const = default;
};
