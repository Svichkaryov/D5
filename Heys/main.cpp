#include "stdafx.h"
#include "HeysCipher.h"

int main()
{
	HeysCipher HC;
	data_t text = { 0x7A1D,3453,123 };
	data_t key = { 123,213,125 };
	HC.permutation(text[0]);
	std::cout << text[0] << std::endl;
	system("pause");
	return 0;
}