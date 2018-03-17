#include "stdafx.h"
#include "HeysCipher/W_HeysCipher.h"
#include "common/const.h"
#include "DifferentialAnalysis/HeysAnalysis.h"
#include "HeysCipher/HeysCipher.h"
#include "common/io.h"


//#define DIFF_SEARCH
#define ATTACK


int main(int argc, char* argv[])
{
	//W_HeysCipher::consoleRun(argc, argv);

	//data_t pt = {};
	//data_t ct = {};
	//FileReader fr;
	//
	//for (int i = 0; i < 65536; i++)
	//{
	//	pt.push_back(i);
	//}
	//fr.setDataBlock(pt, path::pathToTestFolder + "pt.txt");
	//HeysCipher HC(4);
	//HC.run(ENCRYPT, pt, ct);
	//fr.setDataBlock(ct, path::pathToTestFolder + "ct.txt");
	
	//	fr.getDataBlock(path::pathToTestSVFolder + "ct.txt", encryptedBlocks);
//	fr.getDataBlock(path::pathToTestSVFolder + "pt.txt", test);

#ifdef DIFF_SEARCH


	std::map<int, double> result;
	result = HeysAnalysis::differentialSearch(0x5000, 4);


	typedef std::function<bool(std::pair<int, double>, std::pair<int, double>)> Comparator;

	Comparator compFunctor = [](std::pair<int, double> elem1, std::pair<int, double> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<int, double>, Comparator> setOfDiff(result.begin(), result.end(), compFunctor);



	for (std::pair<int, double> element : setOfDiff)
		printf("%x :: %f\n",element.first,element.second);

	

#endif // difSearch


#ifdef ATTACK

	std::cout << HeysAnalysis::attackAttempt(4, 0x5000, 0x4404, 0.0002);

#endif // attack

	system("pause");
	return 0;

}