#include "stdafx.h"
#include "io.h"
#include <sstream>



std::string FileReader::getFileName(const std::string& filePath, bool withExtension, char seperator)
{
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
		bytes.pop_back();

	for (int i = 0; i < bytes.size(); i += 2)
	{
		block_t high   = (static_cast<block_t>(bytes[i + 1]) & 0xFF) << 8;
		block_t low    = static_cast<block_t>(bytes[i]) & 0xFF;
		block_t _block = high ^ low;

		to.push_back(_block);
	}

	return 1;
}


int FileReader::setDataBlock(const data_t& from, const std::string& to)
{
	std::ofstream out(to, std::ios::binary);
	if (!out) 
	{
		printf("Can't open %s file.\n", getFileName(to).c_str());
		return -1;
	}

	for (int i = 0; i < from.size(); ++i)
	{
		block_t temp = from[i];
		byte_t high = static_cast<byte_t>((temp & 0xFF00) >> 8);
		byte_t low  = static_cast<byte_t>(temp & 0xFF);
		out << low << high;
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


std::string FileReader::readLineFromFile(const std::string& sFilename, int lineNumber)
{
	std::string line;
	std::ifstream ifs(sFilename, std::ifstream::in);

	if (!ifs.is_open())
	{
		printf("Can't open file.\n");
		exit(1);
	}

	char n;
	int i = 0;

	while (ifs.good())
	{
		n = ifs.get();
		if (i == lineNumber)
		{
			line += n;
		}
		if (n == '\n')
		{
			i++;
		}
	}

	line.erase(line.size() - 1);
	ifs.close();
	
	return line;
}

std::tuple<int,int,double> FileReader::split(const std::string &s, char delim) 
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	std::tuple<int, int, double> params = std::make_tuple(atoi(elems[0].c_str()),atoi(elems[1].c_str()),atof(elems[2].c_str()));
	
	return params;
}