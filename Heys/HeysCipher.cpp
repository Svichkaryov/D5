#include "stdafx.h"
#include "HeysCipher.h"



void HeysCipher::run(char action, data_t& input, data_t& output)
{

}


void HeysCipher::additionWithKey(block_t& block, const key_t& key)
{
	block = block ^ key;
}


void HeysCipher::substitution(block_t& block)
{
	auto _sBox = getSBox();
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
	block |= (temp_block & 0x842)  << 3;
	block |= (temp_block & 0x84)   << 6;
	block |= (temp_block & 0x8)    << 9;
	block |= (temp_block & 0x4210) >> 3;
	block |= (temp_block & 0x2100) >> 6;
	block |= (temp_block & 0x1000) >> 9;
}

block_t HeysCipher::encrypt(const block_t& block)
{
	return block_t();
}

block_t HeysCipher::decrypt(const block_t& block)
{
	return block_t();
}

std::array<nibble_t, 16> HeysCipher::getSBox()
{
	return sBox;
}
