#pragma once
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::pair;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;
namespace fs = std::filesystem;
using std::exception;
using std::fstream;
using std::ifstream;
using std::map;
using std::ofstream;
using std::stack;

vector<string> split(string s, string delimiter);
