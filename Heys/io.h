#pragma once


#include "common.h"



class FileReader
{
public:
	FileReader() = default;
	~FileReader() = default;

	std::vector<char> readAllBytes(const char* filename);
	int getFileSize(const char* from);
	int setDataBlock(data_t& from, const char* to);
	int getDataBlock(const char* from, data_t& to);
};