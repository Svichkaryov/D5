#pragma once

#include <iostream>
#include <stdint.h>
#include <array>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <string>
#include <iterator>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <functional>


using byte_t     = uint8_t;
using nibble_t   = uint8_t;             
using block_t    = uint16_t; 
using roundKey_t = uint16_t;
using data_t     = std::vector<block_t>;
using key_t      = std::array<roundKey_t, 7>;

enum mode_t { ENCRYPT, DECRYPT };

enum cipherParam {NUMBER_ROUNDS_P, BLOCK_SIZE_P, ROUND_KEY_SIZE_P, KEY_SIZE_P, NUMBER_SUB_KEY_P};