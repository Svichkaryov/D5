#include "../stdafx.h"
#include <iostream>


template<class Function, typename funcNameType, typename... Args>
auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
-> typename std::enable_if<
	!std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
	decltype(f(std::forward<Args>(args)...))>::type
{
	auto t1 = std::chrono::high_resolution_clock::now();
	auto res = f(std::forward<Args>(args)...);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	auto hours = elapsed_time / 3600;
	auto minutes = elapsed_time / 60;
	auto seconds = elapsed_time % 60;
	printf("Elapsed time of %s ( %s ) : %d::%d::%d\n", funcName, typeid(f).name(),
		hours, minutes, seconds);

	return res;
}

template<class Function, typename funcNameType, typename... Args>
auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
-> typename std::enable_if<
	std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
	std::string>::type
{
	auto t1 = std::chrono::high_resolution_clock::now();
	f(std::forward<Args>(args)...);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	auto hours = elapsed_time / 3600;
	auto minutes = elapsed_time / 60;
	auto seconds = elapsed_time % 60;
	printf("Elapsed time of %s ( %s ) : %d::%d::%d\n", funcName, typeid(f).name(),
		hours, minutes, seconds);

	return "Nothing to return (returning type - void)";
}