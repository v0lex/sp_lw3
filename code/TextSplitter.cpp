#include "TextSplitter.h"
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <algorithm>

TextSplitter::TextSplitter() {
	this->text = "";
	this->delim = "";
}

TextSplitter::TextSplitter(std::string text, std::string delim) {
	this->text = text;
	this->delim = delim;
}

int wordLength(const std::string &word) {
	return std::vector(word.begin(), word.end()).size();
}

void deleteEmpties(std::vector<std::string>& words) {
	auto end = std::remove_if(words.begin(), words.end(), [](const std::string& elem) {
		auto pos = elem.find_first_not_of(' ');
		return (pos == std::string::npos) || wordLength(elem) > 30;
		});
	words.erase(end, words.end());
}

std::vector<std::string> TextSplitter::split(bool delEmpties) {
	std::vector<std::string> splitResult;
	boost::algorithm::split(splitResult, text, boost::is_any_of(delim));
	if (delEmpties) {
		deleteEmpties(splitResult);
	}
	return splitResult;
}

