#pragma once

#include "../common/const.h"



class HeysAnalysis
{
public:
	HeysAnalysis() = default;
	~HeysAnalysis() = default;

	static void substituion(mode_t mode, block_t& block, int sBoxNumber);
	static void permutation(block_t& block);
	
	static void calcDPforSBox(std::array<std::array<double, 16>, 16>& dPSBox, int sBoxNumber);
	static void printDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox);
	static int  saveDPForSBox(std::array<std::array<int, 16>, 16>& dPSBox, const std::string& filename);

	static void calcFullDPTable(std::vector<std::vector<int>>& dPTable, int sBoxNumber, const std::string& filename);
	static void calcLineOfDPTable(std::vector<int>& lineOfDPTable, int differential, int sBoxNumber);

	static std::map<int,double> differentialSearch(int a, int sBoxNumber);
	static int attackAttempt(int sBoxNumber, int inputDiff, std::vector<int> differential, double probabilities);

	static std::vector<int> getMostProbDiff(int inputDiff, int diffsNumber, int sBoxNumber);
	static void printDifferentials(std::map<int, double>& resultDiffs);
	static void encryptAllTextsWithMyDefaultKey(int sBoxNumber, const std::string& to);
	static bool isNFragmentsActive(int differential, int numberOfFragments);
private:
	
};
