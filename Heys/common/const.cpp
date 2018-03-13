#include "stdafx.h"
#include "const.h"



namespace path
{
	const std::string pathToTestFolder   = "E:/VS_Proj/SV/Heys/Heys/Heys/test/testMyProg/";
	const std::string pathToTestSVFolder = "E:/VS_Proj/SV/Heys/Heys/Heys/test/testSVProg/";
	const std::string plaintText         = pathToTestFolder + "pt.txt";
	const std::string cipherText         = pathToTestFolder + "ct.txt";
	const std::string key                = pathToTestFolder + "key.txt";
}

namespace sBoxes
{
	// sBoxes is in even position & invSboxes is in odd position
	const std::array<std::array<nibble_t, 16>, 32> sBoxes =
	{
		{
			{ 0xA, 0x9, 0xD, 0x6, 0xE, 0xB, 0x4, 0x5, 0xF, 0x1, 0x3, 0xC, 0x7, 0x0, 0x8, 0x2 },  // 1
	        { 0xD, 0x9, 0xF, 0xA, 0x6, 0x7, 0x3, 0xC, 0xE, 0x1, 0x0, 0x5, 0xB, 0x2, 0x4, 0x8 },  // inv_1

	        { 0x8, 0x0, 0xC, 0x4, 0x9, 0x6, 0x7, 0xB, 0x2, 0x3, 0x1, 0xF, 0x5, 0xE, 0xA, 0xD },  // 2
			{ 0x1, 0xA, 0x8, 0x9, 0x3, 0xC, 0x5, 0x6, 0x0, 0x4, 0xE, 0x7, 0x2, 0xF, 0xD, 0xB },  // inv_2

	        { 0xF, 0x6, 0x5, 0x8, 0xE, 0xB, 0xA, 0x4, 0xC, 0x0, 0x3, 0x7, 0x2, 0x9, 0x1, 0xD },  // 3
			{ 0x9, 0xE, 0xC, 0xA, 0x7, 0x2, 0x1, 0xB, 0x3, 0xD, 0x6, 0x5, 0x8, 0xF, 0x4, 0x0 },  // inv_3

	        { 0x3, 0x8, 0xD, 0x9, 0x6, 0xB, 0xF, 0x0, 0x2, 0x5, 0xC, 0xA, 0x4, 0xE, 0x1, 0x7 },  // 4
			{ 0x7, 0xE, 0x8, 0x0, 0xC, 0x9, 0x4, 0xF, 0x1, 0x3, 0xB, 0x5, 0xA, 0x2, 0xD, 0x6 },  // inv_4

	        { 0xF, 0x8, 0xE, 0x9, 0x7, 0x2, 0x0, 0xD, 0xC, 0x6, 0x1, 0x5, 0xB, 0x4, 0x3, 0xA },  // 5
	        { 0x6, 0xA, 0x5, 0xE, 0xD, 0xB, 0x9, 0x4, 0x1, 0x3, 0xF, 0xC, 0x8, 0x7, 0x2, 0x0 },  // inv_5
	        
	        { 0x2, 0x8, 0x9, 0x7, 0x5, 0xF, 0x0, 0xB, 0xC, 0x1, 0xD, 0xE, 0xA, 0x3, 0x6, 0x4 },  // 6
	        { 0x6, 0x9, 0x0, 0xD, 0xF, 0x4, 0xE, 0x3, 0x1, 0x2, 0xC, 0x7, 0x8, 0xA, 0xB, 0x5 },  // inv_6
	        
	        { 0x3, 0x8, 0xB, 0x5, 0x6, 0x4, 0xE, 0xA, 0x2, 0xC, 0x1, 0x7, 0x9, 0xF, 0xD, 0x0 },  // 7
	        { 0xF, 0xA, 0x8, 0x0, 0x5, 0x3, 0x4, 0xB, 0x1, 0xC, 0x7, 0x2, 0x9, 0xE, 0x6, 0xD },  // inv_7
	        
	        { 0x1, 0x2, 0x3, 0xE, 0x6, 0xD, 0xB, 0x8, 0xF, 0xA, 0xC, 0x5, 0x7, 0x9, 0x0, 0x4 },  // 8
	        { 0xE, 0x0, 0x1, 0x2, 0xF, 0xB, 0x4, 0xC, 0x7, 0xD, 0x9, 0x6, 0xA, 0x5, 0x3, 0x8 },  // inv_8
	        
	        { 0xE, 0x9, 0x3, 0x7, 0xF, 0x4, 0xC, 0xB, 0x6, 0xA, 0xD, 0x1, 0x0, 0x5, 0x8, 0x2 },  // 9
	        { 0xC, 0xB, 0xF, 0x2, 0x5, 0xD, 0x8, 0x3, 0xE, 0x1, 0x9, 0x7, 0x6, 0xA, 0x0, 0x4 },  // inv_9
	        
	        { 0xA, 0xD, 0xC, 0x7, 0x6, 0xE, 0x8, 0x1, 0xF, 0x3, 0xB, 0x4, 0x0, 0x9, 0x5, 0x2 },  // 10
	        { 0xC, 0x7, 0xF, 0x9, 0xB, 0xE, 0x4, 0x3, 0x6, 0xD, 0x0, 0xA, 0x2, 0x1, 0x5, 0x8 },  // inv_10
	        
	        { 0x4, 0xB, 0x1, 0xF, 0x9, 0x2, 0xE, 0xC, 0x6, 0xA, 0x8, 0x7, 0x3, 0x5, 0x0, 0xD },  // 11
	        { 0xE, 0x2, 0x5, 0xC, 0x0, 0xD, 0x8, 0xB, 0xA, 0x4, 0x9, 0x1, 0x7, 0xF, 0x6, 0x3 },  // inv_11
	        
	        { 0x4, 0x5, 0x1, 0xC, 0x7, 0xE, 0x9, 0x2, 0xA, 0xF, 0xB, 0xD, 0x0, 0x8, 0x6, 0x3 },  // 12 
	        { 0xC, 0x2, 0x7, 0xF, 0x0, 0x1, 0xE, 0x4, 0xD, 0x6, 0x8, 0xA, 0x3, 0xB, 0x5, 0x9 },  // inv_12
	        
	        { 0xC, 0xB, 0x3, 0x9, 0xF, 0x0, 0x4, 0x5, 0x7, 0x2, 0xE, 0xD, 0x1, 0xA, 0x8, 0x6 },  // 13
	        { 0x5, 0xC, 0x9, 0x2, 0x6, 0x7, 0xF, 0x8, 0xE, 0x3, 0xD, 0x1, 0x0, 0xB, 0xA, 0x4 },  // inv_13
	        
	        { 0x8, 0x7, 0x3, 0xA, 0x9, 0x6, 0xE, 0x5, 0xD, 0x0, 0x4, 0xC, 0x1, 0x2, 0xF, 0xB },  // 14
	        { 0x9, 0xC, 0xD, 0x2, 0xA, 0x7, 0x5, 0x1, 0x0, 0x4, 0x3, 0xF, 0xB, 0x8, 0x6, 0xE },  // inv_14
	        
	        { 0xF, 0x0, 0xE, 0x6, 0x8, 0xD, 0x5, 0x9, 0xA, 0x3, 0x1, 0xC, 0x4, 0xB, 0x7, 0x2 },  // 15
	        { 0x1, 0xA, 0xF, 0x9, 0xC, 0x6, 0x3, 0xE, 0x4, 0x7, 0x8, 0xD, 0xB, 0x5, 0x2, 0x0 },  // inv_15
	        
	        { 0x4, 0x3, 0xE, 0xD, 0x5, 0x0, 0x2, 0xB, 0x1, 0xA, 0x7, 0x6, 0x9, 0xF, 0x8, 0xC },  // 16
	        { 0x5, 0x8, 0x6, 0x1, 0x0, 0x4, 0xB, 0xA, 0xE, 0xC, 0x9, 0x7, 0xF, 0x3, 0x2, 0xD }   // inv_16
		}
	};


	const std::array<nibble_t, 16> sBox4 = { 0x3, 0x8, 0xD, 0x9, 0x6, 0xB, 0xF, 0x0, 0x2, 0x5, 0xC, 0xA, 0x4, 0xE, 0x1, 0x7 };
	const std::array<nibble_t, 16> invSBox4 = { 0x7, 0xE, 0x8, 0x0, 0xC, 0x9, 0x4, 0xF, 0x1, 0x3, 0xB, 0x5, 0xA, 0x2, 0xD, 0x6 };

	std::array<nibble_t, 16> getInvSbox(const std::array<nibble_t, 16>& sBox)
	{
		std::array<nibble_t, 16> invSBox;

		for (int i = 0; i < sBox.size(); i++)
		{
			invSBox[sBox[i]] = i;
		}

		// printSBox(invSBox);

		return invSBox;

	}


	void printSBox(const std::array<nibble_t, 16>& sBox)
	{
		for (auto& el : sBox)
		{
			printf("0x%1X ", el);
		}
		printf("\n");
	}

}