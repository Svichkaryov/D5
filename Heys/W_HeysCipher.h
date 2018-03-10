#pragma once

#include "common.h"

class W_HeysCipher
{
public:
	W_HeysCipher() = default;
	~W_HeysCipher() = default;

	void run(mode_t mode, const char* from, const char* to, const char* key);

private:

};
