#include "stdafx.h"
#include "HeysLinearAnalysis.h"
#include "../common/io.h"
#include "../HeysCipher/HeysCipher.h"
#include "../DifferentialAnalysis/HeysDiffAnalysis.h"



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
	block |= (temp_block & 0x0842) << 3;
	block |= (temp_block & 0x0084) << 6;
	block |= (temp_block & 0x0008) << 9;
	block |= (temp_block & 0x4210) >> 3;
	block |= (temp_block & 0x2100) >> 6;
	block |= (temp_block & 0x1000) >> 9;
}


void HeysLinearAnalysis::calcLineLinearApproxTable(std::vector<uint32_t>& linelATable, int alfa, int sBoxNumber)
{
	const int BLOCK_SIZE        = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	const int ROUNDS_NUMBER     = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	const int BLOCKS_NUMBER     = 1 << BLOCK_SIZE;
	const int HALF_BLOCK_NUMBER = BLOCKS_NUMBER >> 1;

	std::array<int, 16> scalarMul = {};
	for (int i = 0; i < 16; ++i)
	{
		int ctr = 0;
		for (int j = 0; j < 16; ++j)
		{
			if ((i >> j) & 1)
				ctr++;
		}
		scalarMul[i] = ctr&1;
	}

	int lASBox[16][16];
	std::array<nibble_t, 16>sbox = sBoxes::sBoxes[2 * sBoxNumber - 2];

	for (int alfa = 0; alfa < 16; ++alfa)
	{
		for (int beta = 0; beta < 16; ++beta)
		{
			lASBox[alfa][beta] = 0;
			for (int x = 0; x < 16; ++x)
			{
				lASBox[alfa][beta] += scalarMul[alfa&x] ^ scalarMul[beta&sbox[x]];
			}
		}
	}

	int alfa_0 = (alfa >> 0) & 0xF;
	int alfa_1 = (alfa >> 4) & 0xF;
	int alfa_2 = (alfa >> 8) & 0xF;
	int alfa_3 = (alfa >> 12) & 0xF;

	for (int beta = 0; beta < BLOCKS_NUMBER; ++beta)
	{
		int beta_0 = (beta >> 0) & 0xF;
		int beta_1 = (beta >> 4) & 0xF;
		int beta_2 = (beta >> 8) & 0xF;
		int beta_3 = (beta >> 12) & 0xF;

		// piling-up lemma :
		// Pr(x_1 ^ x_2 ^ x_3 ^ x_4 = 1) = |x_i,i={1,..,4}--independent|=
		// = Pr(x_1 = 1, x_2 = 0, x_3 = 0, x_4 = 0) + ... + 
		// + Pr(x_1 = 0, x_2 = 1, x_3 = 1, x_4 = 1) + ... = ...
		// Pr(b = 1) = p, Pr(b = 0) = 1 - p; b~Bernoulli(p), p=1/2 + s = (1-e)/2

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

		uint16_t numeratorOfProbability = 0;
		/*
		numeratorOfProbability = (countOfSingle_0 - countOfZero_0) * (countOfSingle_0 - countOfZero_0) *
			(countOfSingle_1 - countOfZero_1) * (countOfSingle_1 - countOfZero_1) *
			(countOfSingle_2 - countOfZero_2) * (countOfSingle_2 - countOfZero_2) *
			(countOfSingle_3 - countOfZero_3) * (countOfSingle_3 - countOfZero_3);
		*/
		
		
		numeratorOfProbability += countOfSingle_0 * countOfZero_1   * countOfZero_2   * countOfZero_3;
		numeratorOfProbability += countOfZero_0   * countOfSingle_1 * countOfZero_2   * countOfZero_3;
		numeratorOfProbability += countOfZero_0   * countOfZero_1   * countOfSingle_2 * countOfZero_3;
		numeratorOfProbability += countOfZero_0   * countOfZero_1   * countOfZero_2   * countOfSingle_3;

		numeratorOfProbability += countOfZero_0   * countOfSingle_1 * countOfSingle_2 * countOfSingle_3;
		numeratorOfProbability += countOfSingle_0 * countOfZero_1   * countOfSingle_2 * countOfSingle_3;
		numeratorOfProbability += countOfSingle_0 * countOfSingle_1 * countOfZero_2   * countOfSingle_3;
		numeratorOfProbability += countOfSingle_0 * countOfSingle_1 * countOfSingle_2 * countOfZero_3;
		
		if (numeratorOfProbability != HALF_BLOCK_NUMBER)
		{
			block_t _block = static_cast<block_t>(beta);
			permutation(_block);
			linelATable.push_back(_block | (numeratorOfProbability << BLOCK_SIZE));
		}
	}
}


std::map<int, double> HeysLinearAnalysis::linearApproximationsSearch(int alfa, int sBoxNumber)
{
	int BLOCK_SIZE    = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int ROUNDS_NUMBER = HeysCipher::getCipherParam(NUMBER_ROUNDS_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::map<int, double> result;
	
	double prob[5] = { 0.00015, 0.00015, 0.00015, 0.00015, 0.000015 }; // emperical probabilities for i_sBox

	double* currentListPotentials = new double[BLOCKS_NUMBER];
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		currentListPotentials[i] = -1.0;
	}

	currentListPotentials[alfa] = 1.0;

	double* nextListPotentials = new double[BLOCKS_NUMBER];

	for (int round = 1; round < 6; ++round)
	{
		//printf("round %d:\n", round);
		for (int i = 0; i < BLOCKS_NUMBER; ++i)
		{
			nextListPotentials[i] = -1;
		}

		for (int inputBlock = 0; inputBlock < BLOCKS_NUMBER; ++inputBlock)
		{
			double inputPotential = currentListPotentials[inputBlock];

			if (inputPotential < 0.0) {
				continue;
			}

			std::vector<uint32_t> linelATable;
			calcLineLinearApproxTable(linelATable,inputBlock, sBoxNumber);

			for (uint32_t blockAndNumeratorOfProbability : linelATable)
			{
				uint32_t bANOP = blockAndNumeratorOfProbability;
				int block = bANOP & 0xffff;
				int numeratorOfProbability = bANOP >> 16;

				double currentPotential = nextListPotentials[block];
				if (currentPotential < 0.0)
				{
					currentPotential = 0.0;
				}

				double correlation = 1 - 2 * (static_cast<double>(numeratorOfProbability) / static_cast<double>(BLOCKS_NUMBER));
				double potential = correlation * correlation;
				nextListPotentials[block] = currentPotential + (potential * inputPotential);
			}
		}
		
		
		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			if (nextListPotentials[i] < prob[round - 1])
			{
				nextListPotentials[i] = -1.0;
			}
		}
		
		
		for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
			currentListPotentials[i] = nextListPotentials[i];
		}

		/*for (int i = 0; i < BLOCKS_NUMBER; i++)
		{
		if (currentListPotentials[i] > 0)
		{
		printf("round: %d, diff: %x, prob: %f\n", round, i, currentListPotentials[i]);
		}
		}*/

		if (round == 5)
		{
			for (int i = 0; i < BLOCKS_NUMBER; i++)
			{
				if (currentListPotentials[i] != -1.0)
				{
					result.insert(std::pair<int,double>(i, currentListPotentials[i]));
				}
			}
		}
	}

	delete[] currentListPotentials;
	delete[] nextListPotentials;

	return result;
}


std::vector<std::pair<int,int>> HeysLinearAnalysis::linearAttackAttempt(int sBoxNumber, std::vector<std::tuple<int,int,double>> approxAndLP)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);

	std::vector<double> LP = {};
	for (int i = 0; i < approxAndLP.size(); ++i)
	{
		LP.push_back(std::get<double>(approxAndLP.at(i)));
	}

	auto it_min_LP = std::min_element(std::begin(LP), std::end(LP)); 
	double min_LP = *it_min_LP;

	int textNumber = (int)(1.0 / min_LP);
	printf("textNumber = %d\n", textNumber);

	FileReader fr;
	/*
	data_t allBlocks = {};
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		allBlocks.push_back(i);
	}

	fr.setDataBlock(allBlocks, path::pathToTestFolder + "pt.txt"); // insert all blocks into file pt.txt for encrypt
	*/

	// encrypt data with exe ......
	//HeysDiffAnalysis::encryptAllTextsWithMyDefaultKey(sBoxNumber, path::pathToTestFolder + "ct.txt");

	// continue 

	data_t encryptedBlocks = {};
	fr.getDataBlock(path::pathToTestSVFolder + "ct.txt", encryptedBlocks);

	/*
	key_t m_roundKeys = { 0x32C6, 0x3433, 0x3635, 0x3837, 0x3139, 0x03332, 0x3534 };

	data_t encrypted_1_Blocks = {};
	for (auto bl : encryptedBlocks)
	{
		block_t _block = bl;

		_block = _block ^ m_roundKeys[6];

		for (int i = 5; i >= 1; --i)
		{
			permutation(_block);
			substituion(DECRYPT, _block,4);
			_block = _block ^ m_roundKeys[i];
		}
		encrypted_1_Blocks.push_back(_block);
	}
	*/

	srand(time(0));

	std::vector<int> preparedBlocks = {};
	for (int j = 0; j < textNumber; ++j)
	{
		int block;
		do
		{
			block = rand() % 65536;
		} while ((std::find(preparedBlocks.begin(), preparedBlocks.end(), block) != preparedBlocks.end()) );
	//		&& isNFragmentsActive(block, 0) && isNFragmentsActive(block, 1) && isNFragmentsActive(block, 2));

		preparedBlocks.push_back(block);
	}

	/*
	std::array<int, UINT16_MAX + 1> PS;

	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		PS[block] = 0;

		block_t _block = static_cast<block_t>(block);
		permutation(_block);
		substituion(DECRYPT, _block, sBoxNumber);
		PS[block] = _block;
	}
	*/
	std::array<int, UINT16_MAX + 1> SP;

	for (int block = 0; block < BLOCKS_NUMBER; ++block)
	{
		SP[block] = 0;

		block_t _block = static_cast<block_t>(block);
		substituion(ENCRYPT, _block, sBoxNumber);
		permutation(_block);
		SP[block] = _block;
	}

	auto sort_descent_pair = [](std::pair<int, int> const & a, std::pair<int, int> const & b)
	{
		return a.second != b.second ? a.second > b.second : a.first > b.first;
	};

	auto sort_descent_int = [](int& a, int& b)
	{
		return a > b;
	};

	
	std::ofstream out(path::pathToApproxWithHighLP+"key_u.txt", std::ios::binary);
	if (!out)
	{
		printf("Can't open %s file.\n", fr.getFileName(path::pathToApproxWithHighLP + "key_u.txt"));
		return std::vector<std::pair<int, int>>();
	}
	
	std::vector<int> scalarMul = {};
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		int ctr = 0;
		for (int j = 0; j < 16; ++j)
		{
			if ((i >> j) & 1)
				ctr++;
		}
		scalarMul.push_back(ctr & 1);
	}
	

	std::vector<std::pair<int,int>> keysCand = {};
	
	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		keysCand.push_back(std::pair<int,int>(i,0));
	}
	
	for (int approx = 0; approx < approxAndLP.size(); ++approx)
	{
		printf("approx: %d\n", approx);
		std::vector<std::pair<int, int>> keys_ab = {};
		int alfa = std::get<0>(approxAndLP.at(approx));
		int beta = std::get<1>(approxAndLP.at(approx));

		std::vector<int> u_vec = {};
		for (int key = 0; key < BLOCKS_NUMBER; ++key)
		{
			int countZero = 0;
			for (int block : preparedBlocks)
			{
				int cipherBlock = (int)encryptedBlocks[block];
				//int cipherBLock_dec = PS[cipherBlock ^ key];
				int cipherBLock_enc = SP[block ^ key];
				if (scalarMul[alfa&cipherBLock_enc] ^ scalarMul[beta&cipherBlock])
					countZero++;
			}
		//	int countSingle = BLOCKS_NUMBER - countZero;
			int u = abs(textNumber - countZero - countZero);
			u_vec.push_back(u);
		//	int u = countZero;
		//	out << key << ";" << u << ";" << std::endl;
			keys_ab.push_back(std::pair<int, int>(key, u));
		}
		//std::sort(keys_ab.begin(), keys_ab.end(),sort_descent_pair);
		auto u_m = std::max_element(std::begin(u_vec), std::end(u_vec));
		int u_max = 0.7 * (double)(*u_m);
		for (int i = 0; i < BLOCKS_NUMBER; ++i)
		{
			if (keys_ab[i].second > u_max)
			{
				keysCand[i].second++;
			}
		}
		keys_ab.clear();
		u_vec.clear();
	}

	std::sort(keysCand.begin(), keysCand.end(), sort_descent_pair);

	for (int i = 0; i < BLOCKS_NUMBER; ++i)
	{
		out << std::hex << keysCand[i].first  << ";" << std::dec << keysCand[i].second << "\n";
	}

	/*
	auto keys = keysCand;
	std::sort(keys.begin(), keys.end(),sort_descent_int);
	keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

	std::vector<std::pair<int, int>> l;
	for (auto key : keys)
	{
		int result = std::count(keysCand.begin(), keysCand.end(), key);
		l.push_back(std::pair<int, int>(key, result));
	}

	std::sort(l.begin(), l.end(), sort_descent_pair);
	for (int i = 0; i < 100; ++i)
	{
		std::cout << "key: "<< l[i].first  << " | u= " << l[i].second << std::endl;
		if (l[i].first == 56999)
		{
			std::cout << "YEEEEEEEEEEEEEEEEEEEE\n";
		}
	}
	*/


	return keysCand;
}



std::map<int,double> HeysLinearAnalysis::getApproxWithHighLP(int alfa, int approxsNumber, int sBoxNumber)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);
	double boundary = 8.0 / static_cast<double>(BLOCKS_NUMBER);

	auto resultDiffs = linearApproximationsSearch(alfa, sBoxNumber);

	typedef std::function<bool(std::pair<int, double>, std::pair<int, double>)> Comparator;

	Comparator compFunctor = [](std::pair<int, double> elem1, std::pair<int, double> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<int, double>, Comparator> setOfDiff(resultDiffs.begin(), resultDiffs.end(), compFunctor);

	std::map<int,double> result = {};
	int ctr = 0;

	for (std::pair<int, double> element : setOfDiff)
	{
		if (ctr <= approxsNumber && element.second>boundary)
		{
			result.insert(std::pair<int,double>(element.first,element.second));
		}
	}

	return result;
}


void HeysLinearAnalysis::printApprox(std::map<int, double>& resultApprox)
{
	int BLOCK_SIZE = HeysCipher::getCipherParam(BLOCK_SIZE_P);
	int BLOCKS_NUMBER = (1 << BLOCK_SIZE);
	double boundary = 1.0 / static_cast<double>(BLOCKS_NUMBER);

	typedef std::function<bool(std::pair<int, double>, std::pair<int, double>)> Comparator;

	Comparator compFunctor = [](std::pair<int, double> elem1, std::pair<int, double> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<int, double>, Comparator> setOfDiff(resultApprox.begin(), resultApprox.end(), compFunctor);


	for (std::pair<int, double> element : setOfDiff)
	{
		if (element.second > boundary)
		{
			printf("beta: %x :: LP: %f\n", element.first, element.second);
		}
	}
}



std::vector<std::tuple<int, int, double>> HeysLinearAnalysis::accumulationApproxWithHighLP(const std::string& filename, int sBoxNumber)
{
	int alfa[60] = {
		0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
		0x0010,0x0020,0x0030,0x0040,0x0050,0x0060,0x0070,0x0080,0x0090,0x00A0,0x00B0,0x00C0,0x00D0,0x00E0,0x00F0,
		0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700,0x0800,0x0900,0x0A00,0x0B00,0x0C00,0x0D00,0x0E00,0x0F00,
		0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0x9000,0xA000,0xB000,0xC000,0xD000,0xE000,0xF000
	};
	 
	std::vector<std::tuple<int, int, double>> result = {};
	FileReader fr;
	std::ofstream out(filename, std::ios::binary);
	if (!out)
	{
		printf("Can't open %s file.\n", fr.getFileName(filename));
		return std::vector<std::tuple<int, int, double>>();
	}

	for (int i = 0; i < 60; i++)
	{
		printf("alfa: %d \n", alfa[i]);
		std::map<int,double> betaAndLP = HeysLinearAnalysis::getApproxWithHighLP(alfa[i], 10, sBoxNumber);
		for (auto it = betaAndLP.begin(); it!=betaAndLP.end(); ++it)
		{
			result.push_back(std::make_tuple(alfa[i], it->first, it->second));
			out << alfa[i] << ";" << it->first << ";" << it->second << std::endl;
		}
	}

	out.close();
	return result;
}


bool HeysLinearAnalysis::isNFragmentsActive(int diff, int numberOfFragments)
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

std::vector<std::tuple<int, int, double>> HeysLinearAnalysis::deserealizeApproxWithHighLP(const std::string& filename)
{
	std::vector<std::tuple<int, int, double>> approxLP;
	FileReader fr;
	std::ifstream inFile(filename);
	int line_count = std::count(std::istreambuf_iterator<char>(inFile),
		std::istreambuf_iterator<char>(), '\n');
	for (int i = 0; i < line_count; i++)
	{
		std::string line = fr.readLineFromFile(filename, i);
		auto tuple_i = fr.split(line, ';');
		approxLP.push_back(tuple_i);
	}
	return approxLP;

}