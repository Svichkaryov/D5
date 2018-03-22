#pragma once

#include "RoundKey.h"



class HeysCipher
{
public:
	HeysCipher(int sBoxNumber) : sBox(sBoxes::sBoxes[2*sBoxNumber-2]), invSBox(sBoxes::sBoxes[2*sBoxNumber-1]) {};
	HeysCipher(const RoundKey& roundKeys, int sBoxNumber) 
		: m_roundKeys(roundKeys), sBox(sBoxes::sBoxes[2 * sBoxNumber - 2]), invSBox(sBoxes::sBoxes[2 * sBoxNumber - 1]) {} ;
	~HeysCipher() = default;

	void run(mode_t mode, const data_t& input, data_t& output);
	
	std::array<byte_t, 16> getSBox(mode_t mode);
	static void infoCipher();
	static int getCipherParam(cipherParam param);

private:
	static const int ROUNDS_NUMBER       = 6;
	static const int BLOCK_SIZE          = 16;
	static const int ROUND_KEY_SIZE      = 16;
	static const int KEY_SIZE            = 112;
	static const int N_SUB_KEY           = static_cast<int>(KEY_SIZE / ROUND_KEY_SIZE);
	
	RoundKey m_roundKeys;

	const std::array<nibble_t, 16> sBox;
	const std::array<nibble_t, 16> invSBox;

	void additionWithKey(block_t& block, const roundKey_t& key);
	void substitution(mode_t mode, block_t& block);
	void permutation(block_t& block);

	block_t encrypt_block(const block_t& block);
	block_t decrypt_block(const block_t& block);
	void encrypt(const data_t& input, data_t& output);
	void decrypt(const data_t& input, data_t& output);
};