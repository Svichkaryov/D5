#include "stdafx.h"
#include "HeysCipher/W_HeysCipher.h"
#include "HeysCipher/HeysCipher.h"
#include "common/const.h"
#include "common/io.h"
#include "DifferentialAnalysis/HeysDiffAnalysis.h"
#include "LinearAnalysis/HeysLinearAnalysis.h"



//#define CONSOLE_RUN
//#define TEST_ENC_DEC

//#define DIFF_SEARCH
//#define DIFF_ATTACK

//#define LIN_APPROX_LP_SEARCH
#define LINEAR_ATTACK


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
	
	auto resultDiffs = HeysDiffAnalysis::differentialSearch(0x6000, 4);
	HeysDiffAnalysis::printDifferentials(resultDiffs);
	
#endif // DIFF_SEARCH

#ifdef DIFF_ATTACK

	// key[6]_4_sBox = dea7
	auto diffs = HeysDiffAnalysis::getMostProbDiff(0x6000, 1, 4);
	std::cout << HeysDiffAnalysis::diffAttackAttempt(4, 0x6000, diffs, 0.0005) << std::endl;

#endif // DIFF_ATTACK

#ifdef LIN_APPROX_LP_SEARCH
	
	auto approxLP = HeysLinearAnalysis::accumulationApproxWithHighLP(path::pathToApproxWithHighLP + "approxWighHighLP.txt",4);
	
#endif // LIN_APPROX_LP_SEARCH


#ifdef LINEAR_ATTACK

	printf("Linear attack\n");
	auto approxLP = HeysLinearAnalysis::deserealizeApproxWithHighLP(path::pathToApproxWithHighLP + "approxWighHighLP.txt");
	auto keys = HeysLinearAnalysis::linearAttackAttempt(4, approxLP);

#endif // LINEAR_ATTACK

	system("pause");
	return 0;
}
