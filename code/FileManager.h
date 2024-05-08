#pragma once
#include <vector>
#include<string>
#include <map>
class FileManager {
public:
	FileManager();
	FileManager(std::string, std::string, std::string);
	std::string getPath();
	std::string getName();
	std::string getExtension();
	std::vector<std::string> readFile();
	std::string readFileOneLine();
	std::string getFullPath();
	void writeFile(std::map<std::string, int>);
private:
	std::string path = "";
	std::string name = "";
	std::string extension = "";
};
