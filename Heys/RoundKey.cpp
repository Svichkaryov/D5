#include "stdafx.h"
#include "RoundKey.h"



RoundKey::RoundKey(const data_t& roundKeys)
{
	for (int i = 0; i< roundKeys.size(); ++i)
	{
		m_roundKeys[i] = roundKeys[i];
	}
}


const roundKey_t RoundKey::getRoundKey(int currentRoundKey)
{
	return m_roundKeys[currentRoundKey];
}
