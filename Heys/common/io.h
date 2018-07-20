#pragma once

#include "common.h"


class FileReader
{
public:
	FileReader() = default;
	~FileReader() = default;

	std::string getFileName(const std::string& filePath, bool withExtension = true, char seperator = '/');
	std::vector<char> readAllBytes(const std::string& filename);
	long long getFileSize(const std::string& filename);
	int getDataBlock(const std::string& from, data_t& to);
	int setDataBlock(const data_t& from, const std::string& to);
	std::string readLineFromFile(const std::string& filename, int lineNumber);
	std::tuple<int, int, double> split(const std::string & s, char delim);
private:

};