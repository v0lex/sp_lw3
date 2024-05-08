#pragma once
#include <vector>
#include <string>

class TextSplitter {
public:
	TextSplitter();
	TextSplitter(std::string, std::string = " ");
	std::vector<std::string> split(bool = false);
private:
	std::string text;
	std::string delim;
};