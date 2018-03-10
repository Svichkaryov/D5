#include "stdafx.h"
#include "HeysCipher.h"

//#define TEST_WITHOUT_FILE
#define TEST_WITH_FILE

static std::vector<char> ReadAllBytes(char const* filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}


int setDataBlock(data_t& from, char const* to)
{
	std::ofstream out(to);
	if (!out) {
		std::cout << "Cannot open file.\n";
		return -1;
	}

	for (int i = 0; i<from.size(); ++i)
	{
		block_t temp = from[i];
		block_t high = (temp & 0xFF00) >> 8;
		block_t low = temp & 0xFF;
		out << (char)low << (char)high;
	}

	out.close();
	return 1;
}

int getDataBlock(char const* from, data_t& to)
{
	std::streampos begin, end;
	std::ifstream myfile(from, std::ios::binary);
	if (!myfile)
	{
		std::cout << "Cannot open file.\n";
		return -1;
	}
	begin = myfile.tellg();
	myfile.seekg(0, std::ios::end);
	end = myfile.tellg();
	myfile.close();
	std::cout << "size of " << from << " is: " << (end - begin) << " bytes.\n";

	auto bytes = ReadAllBytes(from);
	if (bytes.size() % 2 == 1)
		bytes.push_back('0');

	for (int i = 0; i<bytes.size(); i += 2)
	{
		block_t high = (static_cast<unsigned>(bytes[i + 1]) & 0xFF) << 8;
		block_t low = static_cast<unsigned>(bytes[i]) & 0xFF;
		block_t _block = high ^ low;
		to.push_back(_block);
	}
	return 1;
}


int main()
{

#ifdef  TEST_WITHOUT_FILE
	HeysCipher HC;
	data_t text = { 0x7A1D,3453,123 };
	data_t dec = {};
	data_t enc = {};
	
	for (auto& b : text)
	{
		std::cout << b << " ";
	}
	std::cout << '\n' << std::endl;
	
	HC.run(ENCRYPT, text, dec);
	
	for (auto& b : dec)
	{
		std::cout << b << " ";
	}
	std::cout << '\n' << std::endl;

	HC.run(DECRYPT, dec, enc);
	
	for (auto& b : enc)
	{
		std::cout << b << " ";
	}

#endif //  TEST_WITHOUT_FILE


#ifdef TEST_WITH_FILE

	HeysCipher HC;
	data_t pt = {};
	data_t ct = {};
	data_t key = {};

	getDataBlock("test/pt.txt", pt);
	getDataBlock("test/key.txt", key);
	HC.setKey(key);

	HC.run(ENCRYPT, pt, ct);

	setDataBlock(ct, "test/ct.txt");
	
#endif // TEST_WITH_FILE

	system("pause");
	return 0;
}