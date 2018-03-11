#include "stdafx.h"
#include "W_HeysCipher.h"
#include "HeysCipher.h"
#include "io.h"


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
