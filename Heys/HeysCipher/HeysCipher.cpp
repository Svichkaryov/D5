#include "stdafx.h"
#include "HeysCipher.h"



void HeysCipher::run(mode_t mode, const data_t& input, data_t& output)
{
	switch (mode)
	{
	case ENCRYPT:
		encrypt(input, output);
		break;
	case DECRYPT:
		decrypt(input, output);
		break;
	default:
		break;
	}
}


std::array<nibble_t, 16> HeysCipher::getSBox(mode_t mode)
{
	switch (mode)
	{
	case ENCRYPT:
		return sBox;
	case DECRYPT:
		return invSBox;
	default:
		return std::array<nibble_t, 16>();
	}
}

void HeysCipher::infoCipher()
{
	std::cout << "Number of rounds: " << ROUNDS_NUMBER << std::endl;
	std::cout << "Size is in bit..."  << std::endl;
	std::cout << "Block size: "       << BLOCK_SIZE << std::endl;
	std::cout << "Round key size: "   << ROUND_KEY_SIZE << std::endl;
	std::cout << "Key size: "         << KEY_SIZE << std::endl;
}

int HeysCipher::getCipherParam(cipherParam param)
{
	switch (param)
	{
	case NUMBER_ROUNDS_P:
		return ROUNDS_NUMBER;
	case BLOCK_SIZE_P:
		return BLOCK_SIZE;
	case ROUND_KEY_SIZE_P:
		return ROUND_KEY_SIZE;
	case KEY_SIZE_P:
		return KEY_SIZE;
	case NUMBER_SUB_KEY_P:
		return N_SUB_KEY;
	default:
		return -1;
	}
}


void HeysCipher::additionWithKey(block_t& block, const roundKey_t& key)
{
	block = block ^ key;
}


void HeysCipher::substitution(mode_t mode, block_t& block)
{
	auto _sBox = getSBox(mode);
	nibble_t iNibble;
	block_t iSBoxOutput;

	iNibble = (block >> 0) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 0);
	block &= 0xFFF0;
	block |= iSBoxOutput;

	iNibble = (block>>4)  & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 4);
	block &= 0xFF0F;
	block |= iSBoxOutput;

	iNibble = (block>>8)  & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 8);
	block &= 0xF0FF;
	block |= iSBoxOutput;

	iNibble = (block>>12) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 12);
	block &= 0x0FFF;
	block |= iSBoxOutput;
}


// i'th bit of j'th nibble -> j'th bit of i'th nibble
void HeysCipher::permutation(block_t& block)
{
	block_t temp_block = block;
	block = 0;

	block |= (temp_block & 0x8421) << 0;
	block |= (temp_block & 0x0842) << 3;
	block |= (temp_block & 0x0084) << 6;
	block |= (temp_block & 0x0008) << 9;
	block |= (temp_block & 0x4210) >> 3;
	block |= (temp_block & 0x2100) >> 6;
	block |= (temp_block & 0x1000) >> 9;
}


block_t HeysCipher::encrypt_block(const block_t& block)
{
	block_t _block = block;

	for (int i = 0; i < ROUNDS_NUMBER; ++i)
	{
		additionWithKey(_block, m_roundKeys.getRoundKey(i));
		substitution(ENCRYPT,_block);
		permutation(_block);
	}
	additionWithKey(_block, m_roundKeys.getRoundKey(ROUNDS_NUMBER));

	return _block;
}


block_t HeysCipher::decrypt_block(const block_t& block)
{
	block_t _block = block;
	
	additionWithKey(_block, m_roundKeys.getRoundKey(ROUNDS_NUMBER));

	for (int i = ROUNDS_NUMBER - 1; i >= 0; --i)
	{
		permutation(_block);
		substitution(DECRYPT,_block);
		additionWithKey(_block, m_roundKeys.getRoundKey(i));
	}
	return _block;
}


void HeysCipher::encrypt(const data_t & input, data_t & output)
{
	for (auto& b : input)
	{
		output.push_back(encrypt_block(b));
	}
}


void HeysCipher::decrypt(const data_t & input, data_t & output)
{
	for (auto& b : input)
	{
		output.push_back(decrypt_block(b));
	}
}