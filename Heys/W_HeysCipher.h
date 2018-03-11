#pragma once

#include "common.h"

class W_HeysCipher
{
public:
	W_HeysCipher() = default;
	~W_HeysCipher() = default;

	void run(mode_t mode, const std::string& from, const std::string& to, const std::string& key);
	void consoleRun();
private:

};
