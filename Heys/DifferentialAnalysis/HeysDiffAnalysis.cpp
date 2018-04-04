#include "stdafx.h"
#include "HeysDiffAnalysis.h"
#include "../common/io.h"
#include "../HeysCipher/HeysCipher.h"



void HeysDiffAnalysis::substituion(mode_t mode, block_t& block, int sBoxNumber)
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

void HeysDiffAnalysis::permutation(block_t & block)
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

void HeysDiffAnalysis::calcDPforSBox(std::array<std::array<double, 16>, 16>& dPSBox, int sBoxNumber)
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

void HeysDiffAnalysis::printDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox)
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

int HeysDiffAnalysis::saveDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox, const std::string & filename)
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

void HeysDiffAnalysis::calcFullDPTable(std::vector<std::vector<int>>& dPTable, int sBoxNumber, const std::string& filename)
{
	FileReader FR;
	std::ofstream out(filename);
	if (!out)
	{
		printf("Can't open %s file.\n", FR.getFileName(filename).c_str());
	}

	int BLOCK_SIZE =  HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::array<block_t, UINT16_MAX+1> SP;

	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		SP[block] = 0;

		block_t _block = static_cast<block_t>(block);
		substituion(ENCRYPT, _block, sBoxNumber);
		permutation(_block);
		SP[block] = _block;
	}

	for (int alfa = 1; alfa < BLOCKS_NUMBER; ++alfa)
	{
		std::array<int, UINT16_MAX+1> frequence;
		
		for (int i = 0; i < UINT16_MAX + 1; ++i)
		{
			frequence[i] = 0;
		}

		for (int x = 0; x < BLOCKS_NUMBER; ++x)
		{
			frequence[SP[x] ^ SP[x^alfa]] ++;
		}

		dPTable.push_back(std::vector<int>());
		
		for (int b = 0; b < BLOCKS_NUMBER; b++) 
		{
			if (frequence[b] != 0)
			{
				dPTable[alfa-1].push_back((b | (frequence[b] << BLOCK_SIZE)));
				out << (b | (frequence[b] << BLOCK_SIZE)) << ";";
			}
		}
		out << "\n";

	}
	
	out.close();

	printf("Writing data to %s.\n", FR.getFileName(filename).c_str());
}


void HeysDiffAnalysis::calcLineOfDPTable(std::vector<int>& lineOfDPTable, int alfa, int sBoxNumber)
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


std::map<int, double> HeysDiffAnalysis::differentialSearch(int inputDiff, int sBoxNumber)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int ROUNDS_NUMBER = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::map<int, double> result;

	double prob[5] = { 0.124, 0.003, 0.0003, 0.0003, 0.0003 }; // emperical probabilities for 4_sBox
	//double prob[5] = { 0.124, 0.000195, 0.0003, 0.00005, 0.00005 }; // emperical probabilities for 4_sBox


	double* currentListDiffProbs = new double[BLOCKS_NUMBER];
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		currentListDiffProbs[i] = -1.0;
	}

	currentListDiffProbs[inputDiff] = 1.0;

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
			calcLineOfDPTable(roundDiffProbs, inputDiff, sBoxNumber);

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

int HeysDiffAnalysis::diffAttackAttempt(int sBoxNumber, int inputDiff, std::vector<int> diffs, double diffProb)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	int textNumber = (int)(8.0 / (diffProb - 1.0 / BLOCKS_NUMBER));
	printf("textNumber = %d\n",textNumber);

	FileReader fr;

	/*
	data_t allBlocks = {};
	for (int i = 0; i < BLOCKS_NUMBER; i++)
	{
		allBlocks.push_back(i);
	}
	fr.setDataBlock(allBlocks, path::pathToTestSVFolder + "pt.txt"); // insert all blocks into file pt.txt for encrypt
	*/

	// encrypt data with exe ......
	// encryptAllTextsWithMyDefaultKey(4, path::pathToTestFolder + "ct.txt");

	// continue 

	data_t encryptedBlocks = {};
	fr.getDataBlock(path::pathToTestSVFolder + "ct.txt", encryptedBlocks);
	
	srand(time(0));
	
	data_t preparedBlocks = {};
	for (int j = 0; j < textNumber; ++j)
	{
		block_t block;
		do
		{
			block = rand() % 65536;		
		} while ( (std::find(preparedBlocks.begin(), preparedBlocks.end(), block) != preparedBlocks.end()) 
			&& isNFragmentsActive(block, 0) && isNFragmentsActive(block, 1) && isNFragmentsActive(block, 2));

		preparedBlocks.push_back(block);
	}

	std::array<int, UINT16_MAX + 1> PS;

	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		PS[block] = 0;

		block_t _block = static_cast<block_t>(block);
		permutation(_block);
		substituion(DECRYPT, _block, sBoxNumber);
		PS[block] = _block;
	}

	int keyCandidate = -1;
	int candidateCoicidences = 0;

	for (int key = 0; key < BLOCKS_NUMBER; key++)
	{
		int numOfCoincidences = 0;
		
		for (int block : preparedBlocks)
		{
			int blockHatch = block ^ inputDiff;

			int cipherBlock = encryptedBlocks[block];
			int cipherBLockHatch = encryptedBlocks[blockHatch];

			int realDiff = PS[cipherBlock ^ key] ^ PS[cipherBLockHatch ^ key];

			for (auto diff : diffs)
			{
				if (realDiff == diff)
				{
					numOfCoincidences++;
				}
			}
		}

		if (numOfCoincidences > candidateCoicidences)
		{
			candidateCoicidences = numOfCoincidences;
			keyCandidate = key;
		}

		if (key % 1000 == 0)
		{
			printf("%d viewed, candidate: %x, coicidences: %d\n", key, keyCandidate, candidateCoicidences);
		}
	}
	printf("key[6] = %x\n",keyCandidate);
	return keyCandidate;
}


std::vector<int> HeysDiffAnalysis::getMostProbDiff(int inputDiff, int diffsNumber, int sBoxNumber)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);
	double boundary = 4.0 / static_cast<double>(BLOCKS_NUMBER);

	auto resultDiffs = HeysDiffAnalysis::differentialSearch(inputDiff, sBoxNumber);

	typedef std::function<bool(std::pair<int, double>, std::pair<int, double>)> Comparator;

	Comparator compFunctor = [](std::pair<int, double> elem1, std::pair<int, double> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<int, double>, Comparator> setOfDiff(resultDiffs.begin(), resultDiffs.end(), compFunctor);

	std::vector<int> result = {};
	int ctr = 0;
	
	for (std::pair<int, double> element : setOfDiff)
	{
		if(HeysDiffAnalysis::isNFragmentsActive(element.first, 4))
			ctr++;
		
		if(ctr<=diffsNumber && HeysDiffAnalysis::isNFragmentsActive(element.first, 4))
		{
			result.push_back(element.first);
		}
	}

	return result;
}

void HeysDiffAnalysis::printDifferentials(std::map<int, double>& resultDiffs)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);
	double boundary = 64.0 / static_cast<double>(BLOCKS_NUMBER);

	typedef std::function<bool(std::pair<int, double>, std::pair<int, double>)> Comparator;

	Comparator compFunctor = [](std::pair<int, double> elem1, std::pair<int, double> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<int, double>, Comparator> setOfDiff(resultDiffs.begin(), resultDiffs.end(), compFunctor);


	for (std::pair<int, double> element : setOfDiff)
	{
		if (element.second > boundary && HeysDiffAnalysis::isNFragmentsActive(element.first, 4))
		{
			printf("diff: %x :: prob: %f\n", element.first, element.second);
		}
	}
}


void HeysDiffAnalysis::encryptAllTextsWithMyDefaultKey(int sBoxNumber, const std::string& to)
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


bool HeysDiffAnalysis::isNFragmentsActive(int diff, int numberOfFragments)
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