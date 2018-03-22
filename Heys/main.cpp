#include "stdafx.h"
#include "HeysCipher/W_HeysCipher.h"
#include "common/const.h"
#include "DifferentialAnalysis/HeysAnalysis.h"
#include "HeysCipher/HeysCipher.h"
#include "common/io.h"


//#define CONSOLE_RUN
//#define DIFF_SEARCH
//#define TEST_ENC_DEC
#define ATTACK


int main(int argc, char* argv[])
{
#ifdef CONSOLE_RUN

	W_HeysCipher::consoleRun(argc, argv);

#endif // CONSOLE_RUN

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
	
		auto resultDiffs = HeysAnalysis::differentialSearch(0x0009, 4);
		HeysAnalysis::printDifferentials(resultDiffs);
	
#endif // DIFF_SEARCH

#ifdef ATTACK

	// key[6]_4_sBox = dea7
	auto diffs = HeysAnalysis::getMostProbDiff(0x0006, 1, 4);
	std::cout << HeysAnalysis::attackAttempt(4, 0x0006, diffs, 0.0005) << std::endl;

#endif // ATTACK

	system("pause");
	return 0;
}
