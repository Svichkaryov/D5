#pragma once

#include <iostream>
#include <stdint.h>
#include <array>
#include <vector>
#include <string>
#include <iterator>
#include <cassert>
#include <fstream>

using byte_t     = uint8_t;
using nibble_t   = uint8_t;             
using block_t    = uint16_t; 
using subKey_t   = uint16_t;
using data_t     = std::vector<block_t>;
using key_t      = std::array<subKey_t, 7>;


enum mode_t { ENCRYPT, DECRYPT };