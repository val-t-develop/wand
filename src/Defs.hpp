#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <memory>
#include <string>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <exception>

using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::make_shared;
namespace fs = std::filesystem;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::exception;


vector<string> split (string s, string delimiter);