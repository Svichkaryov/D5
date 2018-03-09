#pragma once

#include <iostream>
#include <stdint.h>
#include <array>
#include <vector>
#include <cassert>

using byte_t     = uint8_t;
using nibble_t   = uint8_t;             
using block_t    = uint16_t; 
using key_t      = uint16_t;
using data_t     = std::vector<block_t>;

enum mode_t { ENCRYPT, DECRYPT };