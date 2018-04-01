#pragma once

#include "../common/const.h"



class HeysLinearAnalysis
{
public:
	HeysLinearAnalysis() = default;
	~HeysLinearAnalysis() = default;

	static void calcLinearApproxTable(std::vector<std::vector<int>>& dPTable, int sBoxNumber);

	static void substituion(mode_t mode, block_t& block, int sBoxNumber);
	static void permutation(block_t& block);

	static std::map<int, double> linearApproximationsSearch(int a, int sBoxNumber);
	static int scalarMul(int a, int b);
	static int singleBitCount(int num);
private:

};