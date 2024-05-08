#include "FileManager.h"
#include "TextSplitter.h"
#include "FSM.h"
#include <iostream>
#include <string>
#include <vector>
#include<boost/regex.hpp>
#include <boost/algorithm/string.hpp>

const std::vector<std::string> reservedWords = {
	"break", "case", "const", "catch", "class", "continue", "debugger", "default",
	"delete", "do", "else", "extends", "export", "false", "finally", "for", "function",
	"if", "import", "in", "instanceof", "new", "null", "return", "super","switch",
	"this", "throw", "true", "try", "typeof", "var", "void", "while", "with",
	"let", "static", "yield", "await", "enum", "implements", "interface", "package",
	"private", "protected", "public", "abstract", "boolean", "byte", "char", "double",
	"final", "float", "goto", "int", "long", "native", "short", "synchronized",
	"throws", "transient", "volatile", "arguments", "as", "async", "eval", "from",
	"get", "of", "set"
};

const std::vector<std::string> operators = {
	"+", "-", "~", "!", "**", "*", "/", "%", "<=", ">=", "<", ">", "==", "!=",
	"===", "!==", "<<", ">>",">>>", "&", "|", "^", "&&", "||", "??", "=",
	"*=", "+=","-=","/=", "%=", "<<=", ">>=",">>>=", "&=", "|=", "^=", "&&=", "||=",
	"??=","(", ")", "[", "]", ",", ":", ".","?"
};

const std::vector<std::string> separators = {
	 ";", "{", "}"
};

std::vector<std::string> findLineComments(std::vector<std::string>);
bool identifier(std::string);
bool isArrayInit(std::string);
std::vector<std::pair<std::string, std::string>> findArrayInits(std::string);
bool blockcomment(std::string);
bool decimalnumber(std::string);
bool hexadecimalnumber(std::string);
bool floatingpointnumber(std::string);

int main() {
	/*boost::regex comment_re("//[\\w\\d\\s]+");
	boost::smatch result;
	std::string s = " //csaaffafsa";
	boost::regex_search(s.cbegin(), s.cend(), result, comment_re);
	std::cout << s << std::endl << result[0] << std::endl;//.matched << " " <<*result[0].first << " " <<*result[0].second << std::endl;
	if (!result[0].length()) {
		std::cout << "No matches found" << std::endl;
	}*/
	//std::cout << result << std::endl;
	//FileManager fin("fsm", "", "txt");
	FileManager fin("test_js", "", "txt"), fout("test_js_out", "", "txt");
	//FileManager fin("test_b_1", "", "txt"), fout("test_b_1_out", "", "txt");
	//FileManager fin("test_ukr", "", "txt"), fout("test_ukr_out", "", "txt");
	//FileManager fin("test_pl", "", "txt"), fout("test_pl_out", "", "txt");
	//FileManager fin("test_kr", "", "txt"), fout("test_kr_out", "", "txt");
	//std::string line = fin.readFileOneLine();
	std::vector<std::string> lines = fin.readFile();
	/*std::map<std::string, int> words;
	std::vector<std::vector<std::string>> splitedLines;
	for (const auto& l : lines) {
		splitedLines.push_back(TextSplitter(l, " ,-\t\n\\\"()'`{}[].:;!@#$%&_?|/~<>=+*").split(true));
	}
	for (const auto& spl : splitedLines) {
		for (const auto& word : spl) {
			words[word] += 1;
		}
	}
	fout.writeFile(words);*/
	for (const auto& l : lines) {
		std::cout << l << std::endl;
	}
	for (auto& l : lines) {
		boost::trim_left(l);
		l += " ";
	}
	/*std::vector<std::vector<std::string>> splitedLines;
	for (const auto& l : lines) {
		splitedLines.push_back(TextSplitter(l, "\/\/").split());
	}
	for (const auto& l : splitedLines) {
		if (l.size() > 1) {
			std::cout << "This is line with line comment";
		}
		else {
			std::cout << "Line without comments";
		};
		for (const auto& l1 : l)
		std::cout << l1 << std::endl;
	}*/
	std::vector<std::pair<std::string, std::string>> res;
	std::vector<std::string> lineComments = findLineComments(lines);
	for (int i = 0; i < lines.size(); i++) {
		if (lineComments.at(i).size()) {
			res.push_back(std::make_pair(lines.at(i).substr(0, lines.at(i).size() - lineComments.at(i).size()), "no status"));
			res.push_back(std::make_pair(lineComments.at(i), "comment"));
		}
		else {
			res.push_back(std::make_pair(lines.at(i), "no status"));
		}
	}
	std::string line="";
	std::vector < std::pair < std::string, std::string >> newres;
	for (int i = 0; i < res.size(); i++) {
		if (res.at(i).second._Equal("comment")) {
			newres.push_back(std::make_pair(line, "no status"));
			line = "";
			newres.push_back(res.at(i));
			continue;
		}
		line += res.at(i).first;
	}
	if (line.size()) {
		newres.push_back(std::make_pair(line, "no status"));
	}
	res.clear();
	res = newres;
	newres.clear();
	for (int i = 0; i < res.size(); i++) {
		if (res.at(i).second._Equal("no status")) {
			if (blockcomment(res.at(i).first)) {
				std::vector<std::string> splitRes;
				boost::algorithm::split(splitRes, res.at(i).first, boost::is_any_of("/*"));
				int  k = 2;
				for (int j = 0; j < splitRes.size(); j++) {
					std::string status = "no status";
					if (j == k) {
						splitRes.at(j) = "/*" + splitRes.at(j) + "*/";
						status = "comment";
						k += 4;
					}
					newres.push_back(std::make_pair(splitRes.at(j), status));
				}
			}
			else {
				newres.push_back(res.at(i));
			}
		}
		else {
			newres.push_back(res.at(i));
		}
	}
	res.clear();
	res = newres;
	newres.clear();
	for (int i = 0; i < res.size(); i++) {
		if (res.at(i).second._Equal("no status")) {
			std::vector<std::string> splitRes;
			boost::algorithm::split(splitRes, res.at(i).first, boost::is_any_of("\""));
			if (splitRes.size() > 1) {
				for (int j = 0; j < splitRes.size(); j++) {
					std::string status = "no status";
					if (j % 2) {
						splitRes.at(j) = "\"" + splitRes.at(j) + "\"";
						status = "literal constant";
						newres.push_back(std::make_pair(splitRes.at(j), status));
					}
					else {
						newres.push_back(std::make_pair(splitRes.at(j), status));
					}
				}
			}
			else {
				newres.push_back(res.at(i));
			}
		}
		else {
			newres.push_back(res.at(i));
		}
	}
	res.clear();
	res = newres;
	newres.clear();
	auto end = std::remove_if(res.begin(), res.end(), [](const std::pair<std::string, std::string> el) {
		auto pos = el.first.find_first_not_of(' ');
		return (pos == std::string::npos);
		});
	res.erase(end, res.end());
	for (int i = 0; i < res.size(); i++) {
		if (res.at(i).second._Equal("no status")) {
			std::vector<std::string> splitRes;
			boost::algorithm::split(splitRes, res.at(i).first, boost::is_any_of(" "));
			auto end = std::remove_if(splitRes.begin(), splitRes.end(), [](const std::string& el) {
				auto pos = el.find_first_not_of(' ');
				return (pos == std::string::npos);
				});
			splitRes.erase(end, splitRes.end());
			for (int j = 0; j < splitRes.size(); j++) {
				std::string status = "no status";
				for (const auto& el : reservedWords) {
					if (splitRes.at(j)._Equal(el)) {
						status = "reserved word";
						break;
					}
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				for (const auto& el : separators) {
					if (splitRes.at(j)._Equal(el)) {
						status = "separator";
						break;
					}
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				for (const auto& el : operators) {
					if (splitRes.at(j)._Equal(el)) {
						status = "operator";
						break;
					}
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				/*if (decimalnumber(splitRes.at(j))) {
					status = "decimal number";
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				if (hexadecimalnumber(splitRes.at(j))) {
					status = "hexadecimal number";
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				if (floatingpointnumber(splitRes.at(j))) {
					status = "floating point number";
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				if (identifier(splitRes.at(j))) {
					status = "identifier";
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}*/
				if (decimalnumber(splitRes.at(j))) {
					status = "decimal number";
				} else if (hexadecimalnumber(splitRes.at(j))) {
					status = "hexadecimal number";
				} else if (floatingpointnumber(splitRes.at(j))) {
					status = "floating point number";
				} else if (identifier(splitRes.at(j))) {
					status = "identifier";
				}
				if (!status._Equal("no status")) {
					newres.push_back(std::make_pair(splitRes.at(j), status));
					continue;
				}
				else {
					newres.push_back(std::make_pair(splitRes.at(j), "error"));
				}
			}
		}
		else {
			newres.push_back(res.at(i));
		}
	}
	res.clear();
	res = newres;
	newres.clear();
	for (int i = 0; i < res.size(); i++) {
		std::string lexemaclass = "";
		std::string status = res.at(i).second;
		if (status._Equal("comment")) {
			lexemaclass = "COM";
		}
		else if (status._Equal("literal constant")) {
			lexemaclass = "LC";
		}
		else if (status._Equal("decimal number")) {
			lexemaclass = "DN";
		}
		else if (status._Equal("hexadecimal number")) {
			lexemaclass = "HDN";
		}
		else if (status._Equal("separator")) {
			lexemaclass = "SEP";
		}
		else if (status._Equal("operator")) {
			lexemaclass = "OP";
		}
		else if (status._Equal("identifier")) {
			lexemaclass = "IDN";
		}
		else if (status._Equal("reserved word")) {
			lexemaclass = "RW";
		}
		else if (status._Equal("floating point number")) {
			lexemaclass = "FPN";
		}
		else if (status._Equal("error")) {
			lexemaclass = "ERR";
		}
		newres.push_back(std::make_pair(res.at(i).first, lexemaclass));
	}
	std::cout << "_______________________________" << std::endl;
	std::cout << "COM = comment\nLC = literal constant\nDN = decimal number\n";
	std::cout << "HDN = hexadecimal number\nSEP = separator\nOP = operator\n";
	std::cout << "IDN = identifier\nRW = reserved word\nFPN = floating point number\nERR = error\n";
	std::cout << "_______________________________" << std::endl;
	for (const auto& el : newres) {
		std::cout << "<" << el.first << " , " << el.second <<" >"<< std::endl;
	}
	/*for (const auto& l : lines) {
		std::cout << l << std::endl;
	}

	std::vector<std::string> lineComments = findLineComments(lines);
	for (const auto& c : lineComments) {
		std::cout << c << std::endl;
	}*/
	//std::cout << line;
	/*FSM fsm = getFSM(fin);
	std::cout << fsm << std::endl;
	//std::string word = "aaaabba";
	std::string word;
	std::cout << "Enter a word: ";
	std::getline(std::cin, word);
	if (fsm.modelFSM(word)) {
		std::cout << "Word \"" << word << "\" is allowed by this FSM." << std::endl;
	}
	else {
		std::cout << "Word \"" << word << "\" is not allowed by this FSM." << std::endl;
	}*/
	
	system("pause");
}

std::vector<std::string> findLineComments(std::vector<std::string> lines) {
	boost::regex comment_re("//[\\w\\d\\s]+");
	boost::smatch result;
	std::vector<std::string> resvec;
	for (const auto& l : lines) {
		boost::regex_search(l.cbegin(), l.cend(), result, comment_re);
		resvec.push_back(result[0]);
	}
	return resvec;
}

bool identifier(std::string s) {
	boost::regex ident_re("[a-zA-Z_$][\\w\\d_$]*");
	boost::smatch result;
	boost::regex_match(s, result, ident_re);
	return result[0].matched;
}

bool decimalnumber(std::string s) {
	boost::regex ident_re("-?[1-9][0-9]*");
	boost::smatch result;
	boost::regex_match(s, result, ident_re);
	return (result[0].matched) or (s._Equal("0"));
}

bool floatingpointnumber(std::string s) {
	boost::regex ident_re("-?[1-9][0-9]*\\.[0-9]*");
	boost::smatch result1;
	boost::regex_match(s, result1, ident_re);
	boost::regex ident_re1("-?0.[0-9]*");
	boost::smatch result2;
	boost::regex_match(s, result2, ident_re1);
	return result1[0].matched || result2[0].matched;
}

bool hexadecimalnumber(std::string s) {
	boost::regex ident_re("0[xX][0-9a-fA-F]*");
	boost::smatch result;
	boost::regex_match(s, result, ident_re);
	return result[0].matched;
}

/*bool isArrayInit(std::string s) {
	//boost::regex arrayInit_re("\\[([(\\w[\\w\\d_]*)()()],)*(\\w[\\w\\d_]*)\\]");
	boost::regex arrayInit_re1("\\[(\\w[\\w\\d_]*)\\]");
	boost::regex arrayInit_re2("\\[(\\w[\\w\\d_]*,\\s)*");
	boost::regex arrayInit_re3("\\[(\\w[\\w\\d_]*,\\s)*(\\w[\\w\\d_]*)\\]");
	boost::regex arrayInit_re4("\\[\"[\\w\\s\\d]*\"\\]");
	boost::regex arrayInit_re5("\\[\"[\\w\\s\\d]*\"\\]");
	//boost::regex re("\\[\\]");
	boost::smatch result;
	boost::regex_search(s.cbegin(), s.cend(), result, arrayInit_re2);
	return result[0].length();
}

std::vector<std::pair<std::string, std::string>> findArrayInits(std::string s) {
	std::vector<std::string> splitResult;
	std::vector<std::pair<std::string, std::string>> res;
	std::string status;
	boost::algorithm::split(splitResult, s, boost::is_any_of("[]"));
	for (int i = 0; i < splitResult.size(); i++) {
		if (i % 2) {
			splitResult.at(i) = "[" + splitResult.at(i) + "]";
			status = "arrayInit";
		}
		else {
			status = "not arrayInit";
		}
		res.push_back(std::make_pair(splitResult.at(i), status));
	}
	return res;
}*/

bool blockcomment(std::string s) {
	boost::regex bc_re("\\/\\*[\\w\\s\\d]*\\*\\/");
	boost::smatch result;
	boost::regex_search(s.cbegin(), s.cend(), result, bc_re);
	return result[0].length();
}