#include "stdafx.h"
#include "io.h"



std::vector<char> FileReader::readAllBytes(const char* filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	
	if (!ifs)
	{
		printf("Can't open %s file.\n", filename);
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


int FileReader::getFileSize(const char* from)
{
	std::streampos begin, end;
	std::ifstream ifs(from, std::ios::binary);

	if (!ifs)
	{
		std::cout << "Can't open file.\n";
		return -1;
	}

	begin = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	end = ifs.tellg();
	ifs.close();
	int _size = end - begin;
	
	std::cout << "Size of " << from << " is: " << _size << " bytes.\n";
	
	return _size;
}


int FileReader::getDataBlock(const char* from, data_t& to)
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


int FileReader::setDataBlock(data_t& from, const char* to)
{
	std::ofstream out(to);
	if (!out) 
	{
		printf("Can't open %s file.\n",from);
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
	
	return 1;
}

