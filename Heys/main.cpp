#include "stdafx.h"
#include "HeysCipher.h"

int main()
{
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

	system("pause");

	return 0;
}