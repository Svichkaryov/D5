#pragma once

#include <iostream>
#include <stdint.h>
#include <array>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <functional>
#include <chrono>
#include <ctime>
#include <thread>


using byte_t     = uint8_t;
using nibble_t   = uint8_t;             
using block_t    = uint16_t; 
using roundKey_t = uint16_t;
using data_t     = std::vector<block_t>;
using key_t      = std::array<roundKey_t, 7>;

enum mode_t { ENCRYPT, DECRYPT };

enum cipherParam { NUMBER_ROUNDS_P, BLOCK_SIZE_P, ROUND_KEY_SIZE_P, KEY_SIZE_P, NUMBER_SUB_KEY_P };

template<class Function, typename funcNameType, typename... Args>
auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
-> typename std::enable_if<
	!std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
	decltype(f(std::forward<Args>(args)...))>::type;

template<class Function, typename funcNameType, typename... Args>
auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
-> typename std::enable_if<
	std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
	std::string>::type;


#include "common.cpp"