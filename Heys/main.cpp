#include "stdafx.h"
#include "W_HeysCipher.h"
#include "const.h"



int main()
{

	W_HeysCipher WHC;
	WHC.run(DECRYPT, path::cipherText, path::plainText, "default");
	system("pause");

	system("pause");
	return 0;
}

