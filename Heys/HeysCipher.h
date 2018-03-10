#pragma once

#include "common.h"



class HeysCipher
{
public:
	HeysCipher()  = default;
	~HeysCipher() = default;

	std::array<byte_t, 16> getSBox(mode_t mode);
	void infoCipher();

	void run(mode_t mode, data_t& input, data_t& output);

	void setKey(const data_t& key)
	{
		for (int i = 0; i<key.size(); ++i)
		{
			roundKeys[i] = key[i];
		}
	}

private:
	static const int N_ROUNDS            = 6;
	static const int BLOCK_SIZE          = 16;
	static const int ROUND_KEY_SIZE      = 16;
	static const int KEY_SIZE            = 112;
	static const int N_SUB_KEY           = static_cast<int>(KEY_SIZE / ROUND_KEY_SIZE);

	const std::array<nibble_t, 16> sBox     = { 0x3, 0x8, 0xD, 0x9, 0x6, 0xB, 0xF, 0x0, 0x2, 0x5, 0xC, 0xA, 0x4, 0xE, 0x1, 0x7 };
	const std::array<nibble_t, 16> invSBox  = { 0x7, 0xE, 0x8, 0x0, 0xC, 0x9, 0x4, 0xF, 0x1, 0x3, 0xB, 0x5, 0xA, 0x2, 0xD, 0x6 };
	key_t roundKeys                         = { 0x32C6, 0x3433, 0x3635, 0x3837, 0x3139, 0x03332, 0x3534 };

	void additionWithKey(block_t& block, const subKey_t& key);
	void substitution(mode_t mode, block_t& block);
	void permutation(block_t& block);

	block_t encrypt_block(const block_t& block);
	block_t decrypt_block(const block_t& block);
	void encrypt(const data_t& input, data_t& output);
	void decrypt(const data_t& input, data_t& output);
};