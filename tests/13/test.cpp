/*  SPL - Simple Programming Language compiler
*  Copyright (C) 2022-2024  Valentyn Tymchyshyn
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or any
*  later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
*  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
*
*  Part of testing system.
*/

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>

using std::string;
using std::ifstream;
using std::vector;
namespace fs = std::filesystem;

class Path {
    fs::path path;
    string content;

  public:
    Path() {
        path = fs::path();
        content = "";
    }

    Path(string name) {
        path = fs::absolute(fs::path(name));
        content = "";
    }

    Path(fs::path &path) {
        this->path = path;
        content = "";
    }

    string getName() { return fs::absolute(path).string(); }
    string getFilename() { return path.filename(); }
    string readFile() {
        if (content == "") {
            if (!fs::is_regular_file(path)) {
                return "";
            }

            ifstream file(path, std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                return "";
            }

            content = string{std::istreambuf_iterator<char>(file),
                             std::istreambuf_iterator<char>()};

            file.close();

            return content;
        } else {
            return content;
        }
    }

    bool isFile() { return fs::is_regular_file(path); }
    bool isDir() { return fs::is_directory(path); }
    vector<Path> getDirContent() {
        vector<Path> vec{};
        for (auto entry : fs::directory_iterator(path)) {
            vec.push_back(Path(entry.path()));
        }
        return vec;
    }

    Path getParent() {
        path = fs::path(getName());
        return Path(path.parent_path());
    }

    static Path getCurrentDir() { return Path(fs::current_path()); }
};


int main (int argc, char **argv) {
    string output = Path("log/exe.log").readFile();
    string exitCode = Path("log/exe.code.log").readFile();
    if (exitCode.starts_with("0") && output.starts_with("abcdef\n")) {
        system("echo OK > test.result");
    } else {
        system("echo FAIL > test.result");
    }
    return 0;
}