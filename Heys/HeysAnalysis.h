#pragma once



class HeysAnalysis
{
public:
	HeysAnalysis();
	~HeysAnalysis();

	static void substituion(mode_t mode, block_t& block, int sBoxNumber);
	static void permutation(block_t& block);

	virtual void attack() = 0;

private:

};
