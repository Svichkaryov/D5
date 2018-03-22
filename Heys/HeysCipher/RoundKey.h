#pragma once

#include "../common/const.h"


class RoundKey
{
public:
	RoundKey() = default;
	RoundKey(const data_t& roundKeys);
	~RoundKey() = default;

	const roundKey_t getRoundKey(int currentRoundKey);

private:
	key_t m_roundKeys = { 0x32C6, 0x3433, 0x3635, 0x3837, 0x3139, 0x03332, 0x3534 }; 

};