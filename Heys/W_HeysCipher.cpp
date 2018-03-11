#include "stdafx.h"
#include "W_HeysCipher.h"
#include "HeysCipher.h"
#include "io.h"
#include "const.h"


void W_HeysCipher::run(mode_t mode,const std::string& from, const std::string& to, const std::string& key)
{
	data_t pt = {};
	data_t ct = {};
	data_t keys = {};
	FileReader fr;

	switch (mode)
	{
	case ENCRYPT:
		fr.getDataBlock(from, pt);
		if(key=="default")
		{
			RoundKey rkeys;
			HeysCipher HC;
			HC.run(ENCRYPT, pt, ct);
		}
		else
		{
			fr.getDataBlock(key, keys);
			RoundKey rkeys(keys);
			HeysCipher HC(rkeys);
			HC.run(ENCRYPT, pt, ct);
		}

		fr.setDataBlock(ct, to);
		break;
	case DECRYPT:
		fr.getDataBlock(from, ct);
		
		if (key == "default")
		{
			RoundKey rkeys;
			HeysCipher HC;
			HC.run(DECRYPT, ct, pt);
		}
		else
		{
			fr.getDataBlock(key, keys);
			RoundKey rkeys(keys);
			HeysCipher HC(rkeys);
			HC.run(DECRYPT, ct, pt);
		}

		fr.setDataBlock(pt, to);
		break;
	default:
		break;
	}
}

void W_HeysCipher::consoleRun(int argc, char* argv[])
{
	// argc = 5 -> 4 param + \0
	// argc = 6 -> 5 param + \0
	if (argc > 1)
	{

		if (argc == 5)
		{
			if (!strcmp(argv[2], "e"))
			{
				if (!strcmp(argv[1], "sp"))
				{
					W_HeysCipher::run(ENCRYPT, path::pathToTestFolder + argv[3], path::pathToTestFolder + argv[4], "default");
				}
				if (!strcmp(argv[1], "op"))
				{
					W_HeysCipher::run(ENCRYPT, argv[3], argv[4], "default");
				}
			}
			else if (!strcmp(argv[2], "d"))
			{
				if (!strcmp(argv[1], "sp"))
				{
					W_HeysCipher::run(DECRYPT, path::pathToTestFolder + argv[3], path::pathToTestFolder + argv[4], "default");
				}
				if (!strcmp(argv[1], "op"))
				{
					W_HeysCipher::run(DECRYPT, argv[3], argv[4], "default");
				}
			}
		}
		else if (argc == 6)
		{
			if (!strcmp(argv[2], "e"))
			{
				if (!strcmp(argv[1], "sp"))
				{
					W_HeysCipher::run(ENCRYPT, path::pathToTestFolder + argv[3], path::pathToTestFolder + argv[4], path::pathToTestFolder + argv[5]);
				}
				if (!strcmp(argv[1], "op"))
				{
					W_HeysCipher::run(ENCRYPT, argv[3], argv[4], argv[5]);
				}
			}
			else if (!strcmp(argv[2], "d"))
			{
				if (!strcmp(argv[1], "sp"))
				{
					W_HeysCipher::run(DECRYPT, path::pathToTestFolder + argv[3], path::pathToTestFolder + argv[4], path::pathToTestFolder + argv[5]);
				}
				if (!strcmp(argv[1], "op"))
				{
					W_HeysCipher::run(DECRYPT, argv[3], argv[4], argv[5]);
				}
			}
		}
		else printf("Wrong number of param.\n");
	}
	else
	{
		printf("Usage: Heys {sp|op} {e|d} <input file> <output file> [<key file>]\n");
		printf("where: sp(for dev, don't use this) or op - standart path to 3 files or own path\n");
		printf("       input file  - name of file to be encrypted\n");
		printf("       output file - name of ciphertext file\n");
		printf("       key file    - name of 14-byte file with encryption key\n\n");
		printf("* For long names with spaces use \"...\" \n\n");
		printf("Examples: \n\n");
		printf("> Heys op e pt.txt ct.txt\n");
		printf("  -- encrypt file \"pt.txt\" to file \"ct.txt\" using embedded encryption key\n\n");
		printf("> Heys op e ct.txt pt.txt key.txt\n");
		printf("  -- decrypt file \"ct.txt\" to file \"pt.txt\" using encryption key from \"key.txt\" \n\n");
	}
}
