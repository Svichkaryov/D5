#pragma once

#include "common.h"


namespace path
{
	extern const std::string pathToTestFolder;
	extern const std::string pathToTestSVFolder;
	extern const std::string plaintText;
	extern const std::string cipherText;
	extern const std::string key;
}


namespace sBoxes
{
	extern const std::array<std::array<nibble_t, 16>, 32> sBoxes;
	extern const std::array<nibble_t, 16> sBox4;
	extern const std::array<nibble_t, 16> invSBox4;

	extern std::array<nibble_t, 16> getInvSbox(const std::array<nibble_t, 16>& sBox);
	extern void printSBox(const std::array<nibble_t, 16>& sBox);
}