#include "stdafx.h"
#include "HeysCipher.h"
#include "io.h"

//#define TEST_WITHOUT_FILE
#define TEST_WITH_FILE
//#define _ENCRYPT_
#define _DECRYPT_

void test();

int main()
{
	test(); 

	system("pause");
	return 0;
}

void test()
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

	data_t pt = {};
	data_t ct = {};
	data_t key = {};
	FileReader fr;

#ifdef _ENCRYPT_

	fr.getDataBlock("test/pt.txt", pt);
	fr.getDataBlock("test/key.txt", key);
	RoundKey rkeys(key);
	HeysCipher HC(rkeys);

	HC.run(ENCRYPT, pt, ct);

	fr.setDataBlock(ct, "test/ct.txt");

#endif _ENCRYPT_


#ifdef _DECRYPT_

	fr.getDataBlock("test/ct.txt", ct);
	fr.getDataBlock("test/key.txt", key);
	RoundKey rkeys(key);
	HeysCipher HC(rkeys);

	HC.run(DECRYPT, ct, pt);

	fr.setDataBlock(pt, "test/pt.txt");

#endif _DECRYPT_


#endif // TEST_WITH_FILE
	system("pause");
}