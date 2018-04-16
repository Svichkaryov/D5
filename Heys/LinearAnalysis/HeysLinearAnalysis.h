#pragma once

#include "../common/const.h"



class HeysLinearAnalysis
{
public:
	HeysLinearAnalysis() = default;
	~HeysLinearAnalysis() = default;

	static void calcLineLinearApproxTable(std::vector<uint32_t>& linelATable,int alfa, int sBoxNumber);
	static std::map<int, double> linearApproximationsSearch(int alfa, int sBoxNumber);
	static std::vector<std::pair<int, int>> linearAttackAttempt(int sBoxNumber, std::vector<std::tuple<int, int, double>> approxAndLP);
	static std::map<int,double> getApproxWithHighLP(int inputAlfa, int approxsNumber, int sBoxNumber);
	static std::vector<std::tuple<int, int, double>> accumulationApproxWithHighLP(const std::string& filename, int sBoxNumber);
	static std::vector<std::tuple<int, int, double>> deserealizeApproxWithHighLP(const std::string& filename);
	static void printApprox(std::map<int, double>& resultApprox);
private:

};