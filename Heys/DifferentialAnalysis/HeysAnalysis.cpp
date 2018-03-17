#include "stdafx.h"
#include "HeysAnalysis.h"
#include "../common/io.h"
#include "../HeysCipher/HeysCipher.h"
#include <time.h>

void HeysAnalysis::substituion(mode_t mode, block_t& block, int sBoxNumber)
{
	std::array<nibble_t,16> _sBox;
	switch (mode)
	{
	case ENCRYPT:
		_sBox = sBoxes::sBoxes[2 * sBoxNumber - 2];
		break;
	case DECRYPT:
		_sBox = sBoxes::sBoxes[2 * sBoxNumber - 1];
		break;
	default:
		break;
	}
	nibble_t iNibble;
	block_t iSBoxOutput;

	iNibble = (block >> 0) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 0);
	block &= 0xFFF0;
	block |= iSBoxOutput;

	iNibble = (block >> 4) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 4);
	block &= 0xFF0F;
	block |= iSBoxOutput;

	iNibble = (block >> 8) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 8);
	block &= 0xF0FF;
	block |= iSBoxOutput;

	iNibble = (block >> 12) & 0xF;
	iSBoxOutput = (_sBox[iNibble] << 12);
	block &= 0x0FFF;
	block |= iSBoxOutput;
}

void HeysAnalysis::permutation(block_t & block)
{
	block_t temp_block = block;
	block = 0;

	block |= (temp_block & 0x8421) << 0;
	block |= (temp_block & 0x842) << 3;
	block |= (temp_block & 0x84) << 6;
	block |= (temp_block & 0x8) << 9;
	block |= (temp_block & 0x4210) >> 3;
	block |= (temp_block & 0x2100) >> 6;
	block |= (temp_block & 0x1000) >> 9;
}

void HeysAnalysis::calcDPforSBox(std::array<std::array<double, 16>, 16>& dPSBox, int sBoxNumber)
{
	std::array<nibble_t, 16>sbox = sBoxes::sBoxes[2 * sBoxNumber - 2];	

	for (int alfa = 0; alfa < 16; ++alfa)
	{
		for (int beta = 0; beta < 16; ++beta)
		{
			dPSBox[alfa][beta] = 0;
		}
		for (int x = 0; x < 16; ++x)
		{
			dPSBox[alfa][sbox[x] ^ sbox[x ^ alfa]] += 1;
		}
	}

}

void HeysAnalysis::printDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox)
{
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			printf("%2d ", dPSBox[i][j]);
		}
		printf("\n");
	}
}

int HeysAnalysis::saveDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox, const std::string & filename)
{
	FileReader FR;
	std::ofstream out(filename);
	if (!out)
	{
		printf("Can't open %s file.\n", FR.getFileName(filename).c_str());
		return -1;
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			out << dPSBox[i][j] << ";";
		}
		out << "\n";
	}

	out.close();

	if (FR.getFileSize(filename) == 0)
	{
		printf("Something is going wrong.\n");
	}
	else
	{
		printf("Writing data to %s.\n", FR.getFileName(filename).c_str());
	}

	return 1;
}

void HeysAnalysis::calcFullDPTable(std::vector<std::vector<int>>& dPTable, int sBoxNumber, const std::string& filename)
{
	FileReader FR;
	std::ofstream out(filename);
	if (!out)
	{
		printf("Can't open %s file.\n", FR.getFileName(filename).c_str());
	}

	int BLOCK_SIZE =  HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int N_BLOCK = (1 << BLOCK_SIZE);

	std::array<block_t, UINT16_MAX+1> SP;

	for (int block = 0; block < N_BLOCK; ++block)
	{
		SP[block] = 0;

		block_t _block = static_cast<block_t>(block);
		substituion(ENCRYPT, _block, sBoxNumber);
		permutation(_block);
		SP[block] = _block;
	}

	for (int alfa = 1; alfa < N_BLOCK; ++alfa)
	{
		std::array<int, UINT16_MAX+1> counters;
		
		for (int i = 0; i < UINT16_MAX + 1; ++i)
		{
			counters[i] = 0;
		}

		for (int x = 0; x < N_BLOCK; ++x)
		{
			counters[SP[x] ^ SP[x^alfa]] ++;
		}

		dPTable.push_back(std::vector<int>());
		
		for (int b = 0; b < N_BLOCK; b++) 
		{
			if (counters[b] != 0) 
			{
				dPTable[alfa-1].push_back((b | (counters[b] << BLOCK_SIZE)));
				out << (b | (counters[b] << BLOCK_SIZE)) << ";";
			}
		}
		out << "\n";

	}
	
	out.close();

	printf("Writing data to %s.\n", FR.getFileName(filename).c_str());
}


void HeysAnalysis::calcLineOfDPTable(std::vector<int>& lineOfDPTable, int alfa, int sBoxNumber)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::array<int, UINT16_MAX + 1> SP;
	std::array<int, UINT16_MAX + 1> frequence;


	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		SP[block] = 0;
		frequence[block] = 0;

		block_t _block = static_cast<block_t>(block);
		substituion(ENCRYPT, _block, sBoxNumber);
		permutation(_block);
		SP[block] = _block;
	}

	
	for (int x = 0; x < BLOCKS_NUMBER; ++x)
	{
		frequence[SP[x] ^ SP[x^alfa]] ++;
	}

	for (int block = 0; block < BLOCKS_NUMBER; block++)
	{
		if (frequence[block] != 0)
		{
			lineOfDPTable.push_back((block | (frequence[block] << BLOCK_SIZE)));
		//	printf("block : %d :: freq : %d\n", block, frequence[block]);
		}
	}

}


std::map<int, double> HeysAnalysis::differentialSearch(int inputDiff, int sBoxNumber)
{
	int BLOCK_SIZE    = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int ROUNDS_NUMBER = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::map<int, double> result;

	double prob[5] = { 0.124, 0.00195, 0.00031, 0.0002, 0.000011 }; // emperical probabilities

	double* currentDiffProbs = new double[BLOCKS_NUMBER];
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		currentDiffProbs[i] = -1.0;
	}

	currentDiffProbs[inputDiff] = 1.0;

	double* nextDiffProbs = new double[BLOCKS_NUMBER];

	for (int round = 1; round < ROUNDS_NUMBER; round++)
	{
		//printf("round %d:\n", round);
		for (int i = 0; i < BLOCKS_NUMBER; ++i)
		{
			nextDiffProbs[i] = -1;
		}

		for (int inputDiff = 0; inputDiff < BLOCKS_NUMBER; inputDiff++)
		{
			double inputDiffProb = currentDiffProbs[inputDiff];

			if (inputDiffProb < 0.0) {
				continue;
			}

			std::vector<int> roundDiffProbs;
			calcLineOfDPTable(roundDiffProbs, inputDiff, sBoxNumber);

			for (int& blockAndProb : roundDiffProbs)
			{
				int bAP = blockAndProb;
				int diff = bAP & 0xffff;
				int freq = bAP >> 16;

				double currentProb = nextDiffProbs[diff];
				if (currentProb < 0.0)
				{
					currentProb = 0.0;
				}

				nextDiffProbs[diff] = currentProb + (((double)freq / (double)BLOCKS_NUMBER) * inputDiffProb);
			}
		}

		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			if (nextDiffProbs[i] < prob[round - 1])
			{
				nextDiffProbs[i] = -1.0;
			}
		}

		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			currentDiffProbs[i] = nextDiffProbs[i];
		}

		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			if (currentDiffProbs[i] > 0)
			{
				//printf("round: %d, diff: %d, prob: %f\n", round, i, currentDiffProbs[i]);
			}
		}

		if (round == 5) 
		{
			for (int i = 0; i < BLOCKS_NUMBER; i++)
			{
				if (currentDiffProbs[i] != -1.0)
				{
					result.insert(std::pair<int, double>(i, currentDiffProbs[i]));
				}
			}
		}
	}

	delete[] currentDiffProbs;
	delete[] nextDiffProbs;

	return result;
}

int HeysAnalysis::attackAttempt(int sBoxNumber, int inputDiff, int diff, double diffProb)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	//int textNumber = (int)(1.0 / (diffProb - 1.0 / BLOCKS_NUMBER));
	//printf("textNumber = %d ",textNumber);

	int textNumber = 1000;

	FileReader fr;

	data_t allBlocks = {};
	data_t encryptedBlocks = {};

	for (int i = 0; i < BLOCKS_NUMBER; i++)
	{
		allBlocks.push_back(i);
	}
	
	//fr.setDataBlock(allBlocks, path::pathToTestSVFolder + "pt.txt"); // insert all blocks into file pt.txt for encrypt
	
	// encrypt data with exe ......

	// continue 
	fr.getDataBlock(path::pathToTestFolder + "ct.txt", encryptedBlocks);
	
	srand(time(0));
	std::vector<block_t> usedBlocks = {};
	for (int j = 0; j < textNumber; ++j)
	{
		int r = rand() % 65535;
		block_t block = allBlocks[r];
		usedBlocks.push_back(block);
	}

	std::array<int, UINT16_MAX + 1> reversePS;

	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		reversePS[block] = 0;

		block_t _block = static_cast<block_t>(block);
		permutation(_block);
		substituion(DECRYPT, _block, sBoxNumber);
		reversePS[block] = _block;
	}


	int keyCandidate = -1;
	int candidateCoincides = 0;

	for (int key = 0; key < BLOCKS_NUMBER; key++) {

		int coincides = 0;

		for (int block : usedBlocks) {
			int pairBlock = block ^ inputDiff;

			int cipherText = encryptedBlocks[block];
			int pairCipherText = encryptedBlocks[pairBlock];

			int actualDiff = reversePS[cipherText ^ key] ^ reversePS[pairCipherText ^ key];

			if (actualDiff == diff) {
				coincides++;
			}
		}

		if (coincides >= candidateCoincides) {
			candidateCoincides = coincides;
			keyCandidate = key;
		}

		if (key % 1000 == 0) {
			printf("%d done, candidate: %x, candidateCoincides: %d\n", key, keyCandidate, candidateCoincides);
		}
	}

	return keyCandidate;
}


void HeysAnalysis::encryptAllTextsWithMyDefaultKey(int sBoxNumber, const std::string& to)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	data_t pt = {};
	data_t ct = {};
	FileReader fr;

	for (int i = 0; i < BLOCKS_NUMBER; i++)
	{
		pt.push_back(i);
	}
	fr.setDataBlock(pt, path::pathToTestFolder + "pt.txt");
	HeysCipher HC(sBoxNumber);
	HC.run(ENCRYPT, pt, ct);
	fr.setDataBlock(ct, to);
}

bool HeysAnalysis::isNFragmentsActive(int diff, int numberOfFragments)
{
	bool active = false;
	int activeFragments = 0;
	
	for (int i = 0; i < 4; i++)
	{
		active = (diff >> 4 * i) & 0xF;
		if (active) 
			activeFragments++;
	}

	if (numberOfFragments == activeFragments)
		return 1;
	else
		return 0;
}