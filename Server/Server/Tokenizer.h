#pragma once
#include <sstream>
#include <vector>
#include <string>

using std::vector;
using std::string;

class Tokenizer {
public:
	static vector<string> split(const string& str, const string& separator = " ");
};

