#pragma once


#include "common.h"



class FileReader
{
public:
	FileReader() = default;
	~FileReader() = default;

	std::string getFileName(const std::string& filePath, bool withExtension, char seperator);
	std::vector<char> readAllBytes(const std::string& filename);
	long long getFileSize(const std::string& from);
	int getDataBlock(const std::string& from, data_t& to);
	int setDataBlock(const data_t& from, const std::string& to);
private:

};