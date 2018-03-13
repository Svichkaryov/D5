#pragma once

#include "../common/common.h"

class W_HeysCipher
{
public:
	W_HeysCipher() = default;
	~W_HeysCipher() = default;

	static void run(mode_t mode, int sBoxNumber, const std::string& from, const std::string& to, const std::string& key);
	static void consoleRun(int argc, char* argv[]);
private:

};
