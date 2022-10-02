#include "Path.hpp"

Path::Path() {
    path = fs::path();
    content = "";
}

Path::Path(string name) {
    path = fs::path(name);
    content = "";
}

Path::Path(fs::path &path) {
    this->path = path;
    content = "";
}

string Path::getName() {
    return fs::absolute(path).string();
}

string Path::getFilename() {
    return path.filename();
}

string Path::readFile() {
    if(content == "") {
        if(!fs::is_regular_file(path)) {
            return "";
        }

        ifstream file(path, std::ios::in | std::ios::binary);
        if(!file.is_open()) {
            return "";
        }

        content = string {std::istreambuf_iterator<char> (file),
                          std::istreambuf_iterator<char>()
                         };

        file.close();

        return content;
    } else {
        return content;
    }
}

bool Path::isFile() {
    return fs::is_regular_file(path);
}

bool Path::isDir() {
    return fs::is_directory(path);
}

vector<Path> Path::getDirContent() {
    vector<Path> vec {};
    for(auto entry : fs::directory_iterator(path)) {
        vec.push_back(Path(entry.path()));
    }
    return vec;
}

Path Path::getParent() {
    path = fs::path(getName());
    return Path(path.parent_path());
}

Path Path::getCurrentDir() {
    return Path(fs::current_path());
}
