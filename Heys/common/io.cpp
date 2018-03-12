#include "stdafx.h"
#include "io.h"


std::string FileReader::getFileName(const std::string& filePath, bool withExtension = true, char seperator = '/')
{
	// Get last dot position
	std::size_t dotPos = filePath.rfind('.');
	std::size_t sepPos = filePath.rfind(seperator);

	if (sepPos != std::string::npos)
	{
		return filePath.substr(sepPos + 1, filePath.size() - (withExtension || dotPos != std::string::npos ? 1 : dotPos));
	}
	return filePath;
}

std::vector<char> FileReader::readAllBytes(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	
	if (!ifs)
	{
		printf("Can't open %s file.\n", getFileName(filename).c_str());
		return std::vector<char>();
	}
	if (getFileSize(filename) == 0)
	{
		return std::vector<char>();
	}

	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);
	ifs.close();
	
	return result;
}


long long FileReader::getFileSize(const std::string& from)
{
	std::streampos begin, end;
	std::ifstream ifs(from, std::ios::binary);

	if (!ifs)
	{
		printf("Can't open %s file.\n", getFileName(from).c_str());
		return -1;
	}

	begin = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	end = ifs.tellg();
	ifs.close();
	long long _size = end - begin;
	
	printf("Size of %s is %I64d bytes.\n", getFileName(from).c_str(), _size);
	
	return _size;
}


int FileReader::getDataBlock(const std::string& from, data_t& to)
{
	auto bytes = readAllBytes(from);
	
	if (bytes.empty())
	{
		return -1;
	}

	if (bytes.size() % 2 == 1)
		bytes.push_back('0');

	for (int i = 0; i < bytes.size(); i += 2)
	{
		block_t high   = (static_cast<unsigned>(bytes[i + 1]) & 0xFF) << 8;
		block_t low    = static_cast<unsigned>(bytes[i]) & 0xFF;
		block_t _block = high ^ low;

		to.push_back(_block);
	}

	return 1;
}


int FileReader::setDataBlock(const data_t& from, const std::string& to)
{
	std::ofstream out(to);
	if (!out) 
	{
		printf("Can't open %s file.\n", getFileName(to).c_str());
		return -1;
	}

	for (int i = 0; i < from.size(); ++i)
	{
		block_t temp = from[i];
		block_t high = (temp & 0xFF00) >> 8;
		block_t low  = temp & 0xFF;

		out << (char)low << (char)high;
	}

	out.close();
	
	if (getFileSize(to) == 0)
	{
		printf("Something is going wrong.\n");
	}
	else
	{
		printf("Writing data to %s.\n", getFileName(to).c_str());
	}

	return 1;
}