#include "FileManager.h"
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>


FileManager::FileManager() {}

FileManager::FileManager(std::string name, std::string path, std::string extension) {
	this->name = name;
	this->path = path;
	this->extension = extension;
}

std:: string FileManager::getPath() {
	return this->path;
}

std::string FileManager::getName() {
	return this->name;
}

std::string FileManager::getExtension() {
	return this->extension;
}

std::string FileManager::getFullPath() {
	return path + name + "." + extension;
}

std::vector<std::string> FileManager::readFile() {
	std::vector <std::string> lines;
	std::ifstream file;
	file.open(this->getFullPath(),std::ios::in);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
	}
	file.close();
	return lines;
}

std::string FileManager::readFileOneLine() {
	std::vector<std::string> lines = this->readFile();
	std::string line = "";
	for (const auto& l : lines) {
		line += " ";
		line += l;
	}
	return line;
}

void FileManager::writeFile(std::map<std::string, int> lines) {
	std::ofstream file;
	file.open(this->getFullPath(), std::ios::out);
	for (const auto& [key, value] : lines) {
		file << key << ": " << value << '\n';
	}
	file.close();
};