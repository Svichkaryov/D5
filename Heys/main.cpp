#include "stdafx.h"
#include "HeysCipher/W_HeysCipher.h"
#include "HeysCipher/HeysCipher.h"
#include "common/const.h"
#include "common/io.h"
#include "DifferentialAnalysis/HeysDiffAnalysis.h"
#include "LinearAnalysis/HeysLinearAnalysis.h"


void f(int s, std::string g)
{

}

//#define CONSOLE_CIPHER_RUN
//#define TEST_ENC_DEC

//#define DIFF_SEARCH
//#define DIFF_ATTACK

//#define LIN_APPROX_LP_SEARCH
//#define LINEAR_ATTACK

//#define CONSOLE_ATTACK_RUN

#define TEST

int main(int argc, char* argv[])
{
	int alfa[60] = {
		0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
		0x0010,0x0020,0x0030,0x0040,0x0050,0x0060,0x0070,0x0080,0x0090,0x00A0,0x00B0,0x00C0,0x00D0,0x00E0,0x00F0,
		0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700,0x0800,0x0900,0x0A00,0x0B00,0x0C00,0x0D00,0x0E00,0x0F00,
		0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0x9000,0xA000,0xB000,0xC000,0xD000,0xE000,0xF000
};

#ifdef TEST

	//auto res = executeAndGetElapsedTime(f, "", 1, "j");
	FileReader fr;
	try
	{
		fr.getFileSize("C:/data/2.txt");
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n" << __FILE__ << "\n" <<__LINE__ <<  std::endl;
	}

#endif // TEST

	
#ifdef CONSOLE_CIPHER_RUN

	W_HeysCipher::consoleRun(argc, argv);

#endif // CONSOLE_CIPHER_RUN

#ifdef TEST_ENC_DEC

	data_t pt = {};
	data_t ct = {};
	FileReader fr;

	for (int i = 0; i < 65536; i++)
	{
		pt.push_back(i);
	}
	fr.setDataBlock(pt, path::pathToTestSVFolder + "pt.txt");
	
	HeysCipher HC(4);
	HC.run(ENCRYPT, pt, ct);
	fr.setDataBlock(ct, path::pathToTestFolder + "ct.txt");
	

#endif // TEST_ENC_DEC


#ifdef DIFF_SEARCH

	auto resultDiffs = HeysDiffAnalysis::differentialSearch(0x6000, 4);
	HeysDiffAnalysis::printDifferentials(resultDiffs);
	
#endif // DIFF_SEARCH

#ifdef DIFF_ATTACK

	// key[6]_4_sBox = dea7

	HeysDiffAnalysis::diffAttack(6, 0x0008, 4, 0.001);
	

#endif // DIFF_ATTACK

#ifdef LIN_APPROX_LP_SEARCH
	
	auto approxLP = HeysLinearAnalysis::accumulationApproxWithHighLP(path::pathToApproxWithHighLP + "approxWighHighLP.txt",4);
	
#endif // LIN_APPROX_LP_SEARCH


#ifdef LINEAR_ATTACK

	printf("Linear attack\n");
	auto approxLP = HeysLinearAnalysis::deserealizeApproxWithHighLP(path::pathToApproxWithHighLP + "approxWighHighLP.txt");
	auto keys = HeysLinearAnalysis::linearAttackAttempt(1, approxLP);

#endif // LINEAR_ATTACK
	

#ifdef CONSOLE_ATTACK_RUN

	printf("1. Differential attack (Attack time ~ 1 min)\n");
	printf("2. Linear attack (Attack time ~ 15 min)\n");
	printf("0. Exit()\n");

	
	int sBoxNumber, inputDiff, outputDiffCount;
	while (true)
	{
		int attackVariantNumber;
		std::cout << "Choose attack(input number) ";
		std::cin >> attackVariantNumber;
		std::cout << std::endl;

		switch (attackVariantNumber)
		{
		case 1:
			std::cout << "Input 3 param:\n 1)sBoxNumber(1-14),\n 2)input differential in hex(i.e alfa),\n 3)output differential count(1 - is enough but maybe need - 4 - for attack accuracy)\n";
			std::cin >> sBoxNumber >> std::hex >> inputDiff >> std::dec >> outputDiffCount;
			std::cout << std::endl;
			HeysDiffAnalysis::diffAttack(sBoxNumber, inputDiff, outputDiffCount, 0.0008);
			break;
		case 2:
			std::cout << "Oops, in maintenance." << std::endl;
			break;
		case 0:
			system("pause");
			return 0;
		default:
			printf("Bad choose\n\n");
			break;
		}
	}

#endif // CONSOLE_ATTACK_RUN


	system("pause");
	return 0;
}
