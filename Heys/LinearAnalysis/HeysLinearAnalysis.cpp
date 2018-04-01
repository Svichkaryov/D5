#include "stdafx.h"
#include "HeysLinearAnalysis.h"
#include "../common/io.h"
#include "../HeysCipher/HeysCipher.h"


void HeysLinearAnalysis::substituion(mode_t mode, block_t& block, int sBoxNumber)
{
	std::array<nibble_t, 16> _sBox;
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

void HeysLinearAnalysis::permutation(block_t & block)
{
	block_t temp_block = block;
	block = 0;

	block |= (temp_block & 0x8421) << 0;
	block |= (temp_block & 0x8420) << 3;
	block |= (temp_block & 0x8400) << 6;
	block |= (temp_block & 0x8000) << 9;
	block |= (temp_block & 0x4210) >> 3;
	block |= (temp_block & 0x2100) >> 6;
	block |= (temp_block & 0x1000) >> 9;
}


void HeysLinearAnalysis::calcLinearApproxTable(std::vector<std::vector<int>>& lATable, int sBoxNumber)
{
	const int BLOCK_SIZE        = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	const int ROUNDS_NUMBER     = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	const int BLOCKS_NUMBER     = (1 << BLOCK_SIZE);
	const int HALF_BLOCK_NUMBER = BLOCKS_NUMBER >> 1;
	
	int lASBox[16][16];
	std::array<nibble_t, 16>sbox = sBoxes::sBoxes[2 * sBoxNumber - 2];

	for (int alfa = 0; alfa < 16; ++alfa)
	{
		for (int beta = 0; beta < 16; ++beta)
		{
			for (int x = 0; x < 16; ++x)
			{
				block_t _x = static_cast<block_t>(x);
				substituion(ENCRYPT, _x, sBoxNumber);
				lASBox[alfa][beta] += scalarMul(alfa,x) ^ scalarMul(beta,_x);
			}
		}
	}
	
	for (int alfa = 0; alfa < BLOCKS_NUMBER; ++alfa)
	{
		lATable.push_back(std::vector<int>());

		int alfa_0 = (alfa >> 0)  & 0xF;
		int alfa_1 = (alfa >> 4)  & 0xF;
		int alfa_2 = (alfa >> 8)  & 0xF;
		int alfa_3 = (alfa >> 12) & 0xF;
		
		int numeratorOfCorrelation[UINT16_MAX + 1];
		std::vector<int> biasValues;
		int biasCount = 0;
		
		for (int beta = 0; beta < BLOCKS_NUMBER; ++beta)
		{
			int beta_0 = (beta >> 0)  & 0xF;
			int beta_1 = (beta >> 4)  & 0xF;
			int beta_2 = (beta >> 8)  & 0xF;
			int beta_3 = (beta >> 12) & 0xF;

			// for understand next part of code, see -- piling-up lemma :)
			// Pr(x_1 ^ x_2 ^ x_3 ^ x_4 = 1) = |x_i,i={1,..,4}--independent|=
			// = Pr(x_1 = 1, x_2 = 0, x_3 = 0, x_4 = 0) + ... + 
			// + Pr(x_1 = 0, x_2 = 1, x_3 = 1, x_4 = 1) + ... = ...
			// Pr(b = 1) = p, Pr(b = 0) = 1 - p; b~Bernoulli(p)
			
			// Pr(b_i = 1) = p_i, countOfSingle_i = 16*p_i
			int countOfSingle_0 = lASBox[alfa_0][beta_0];
			int countOfSingle_1 = lASBox[alfa_1][beta_1];
			int countOfSingle_2 = lASBox[alfa_2][beta_2];
			int countOfSingle_3 = lASBox[alfa_3][beta_3];

			// Pr(b_i = 0) = 1-p_i, countOfZero_i = 16*(1-p_i)
			int countOfZero_0 = 16 - countOfSingle_0;
			int countOfZero_1 = 16 - countOfSingle_1;
			int countOfZero_2 = 16 - countOfSingle_2;
			int countOfZero_3 = 16 - countOfSingle_3;

			int sum = 0;
			
			sum += countOfSingle_0 * countOfZero_1   * countOfZero_2   * countOfZero_3;
			sum += countOfZero_0   * countOfSingle_1 * countOfZero_2   * countOfZero_3;
			sum += countOfZero_0   * countOfZero_1   * countOfSingle_2 * countOfZero_3;
			sum += countOfZero_0   * countOfZero_1   * countOfZero_2   * countOfSingle_3;

			sum += countOfZero_0   * countOfSingle_1 * countOfSingle_2 * countOfSingle_3;
			sum += countOfSingle_0 * countOfZero_1   * countOfSingle_2 * countOfSingle_3;
			sum += countOfSingle_0 * countOfSingle_1 * countOfZero_2   * countOfSingle_3;
			sum += countOfSingle_0 * countOfSingle_1 * countOfSingle_2 * countOfZero_3;

			numeratorOfCorrelation[beta] = sum;

			if (sum != BLOCKS_NUMBER >> 1)
				biasValues.push_back(0);
				biasCount++;
		}

		if (alfa % 1000 == 0) 
			printf("alfa = %d viewed", alfa);

		int i = 0;

		for (block_t block = 0; block < BLOCKS_NUMBER; block++) 
		{
			int numeratorOfCorrelation_block = numeratorOfCorrelation[block];
			
			if (numeratorOfCorrelation_block != HALF_BLOCK_NUMBER)
			{
				block_t _block = block;
				permutation(_block);
				biasValues[i] = _block | (numeratorOfCorrelation_block << 16);
				i++;
			}
		}

		for (auto bv : biasValues)
		{
			lATable[alfa].push_back(bv);
		}
	}
}


std::map<int, double> HeysLinearAnalysis::linearApproximationsSearch(int a, int sBoxNumber)
{
	int BLOCK_SIZE    = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int ROUNDS_NUMBER = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::map<int, double> result;

	double prob[5] = { 0.124, 0.003, 0.0003, 0.0003, 0.0003 }; // emperical probabilities for 4_sBox

	double* currentListDiffProbs = new double[BLOCKS_NUMBER];
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		currentListDiffProbs[i] = -1.0;
	}

	currentListDiffProbs[a] = 1.0;

	double* nextListDiffProbs = new double[BLOCKS_NUMBER];

	for (int round = 1; round < 6; round++)
	{
		printf("round %d:\n", round);
		for (int i = 0; i < BLOCKS_NUMBER; ++i)
		{
			nextListDiffProbs[i] = -1;
		}

		for (int inputDiff = 0; inputDiff < BLOCKS_NUMBER; inputDiff++)
		{
			double inputDiffProb = currentListDiffProbs[inputDiff];

			if (inputDiffProb < 0.0) {
				continue;
			}

			std::vector<int> roundDiffProbs;
		//	calcLineOfDPTable(roundDiffProbs, inputDiff, sBoxNumber);

			for (int& blockAndProb : roundDiffProbs)
			{
				int bAP = blockAndProb;
				int diff = bAP & 0xffff;
				int freq = bAP >> 16;

				double currentProb = nextListDiffProbs[diff];
				if (currentProb < 0.0)
				{
					currentProb = 0.0;
				}

				nextListDiffProbs[diff] = currentProb + ((static_cast<double>(freq) / static_cast<double>(BLOCKS_NUMBER)) * inputDiffProb);
			}
		}

		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			if (nextListDiffProbs[i] < prob[round - 1])
			{
				nextListDiffProbs[i] = -1.0;
			}
		}

		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			currentListDiffProbs[i] = nextListDiffProbs[i];
		}

		/*for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
		if (currentListDiffProbs[i] > 0)
		{
		printf("round: %d, diff: %x, prob: %f\n", round, i, currentListDiffProbs[i]);
		}
		}*/

		if (round == 5)
		{
			for (int i = 0; i < BLOCKS_NUMBER; i++)
			{
				if (currentListDiffProbs[i] != -1.0)
				{
					result.insert(std::pair<int, double>(i, currentListDiffProbs[i]));
				}
			}
		}
	}

	delete[] currentListDiffProbs;
	delete[] nextListDiffProbs;

	return result;
}

int HeysLinearAnalysis::scalarMul(int a, int b)
{
	return singleBitCount(a & b) & 1;
}


int HeysLinearAnalysis::singleBitCount(int num)
{
	int ctr = 0;
	int bitCount = static_cast<int>(log2(num));

	for (int i = 0; i <= bitCount; i++)
	{
		if (((num >> i) & 1) == 1)
			ctr++;
	}

	return ctr;
}