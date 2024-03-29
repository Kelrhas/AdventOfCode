#pragma once

#define Assert(exp) {bool b = (exp); if(!b) __debugbreak();}
#define TODO(msg) Assert(false);

#include <iostream>

inline void DisplayTime(double seconds)
{
	if (seconds > 100)
	{
		std::cout << seconds / 60 << " min";
	}
	else if (seconds > 1)
	{
		std::cout << seconds << " sec";
	}
	else if (seconds > 0.001)
	{
		std::cout << seconds * 1000 << " ms";
	}
	else
	{
		std::cout << seconds * 1'000'000 << " us";
	}
}


template<typename T>
struct powers
{
	constexpr T operator()(T const &lhs, T const &rhs) const { return (T)std::pow(lhs, rhs); }
};

constexpr int Factorial(int i)
{
	if (i <= 1)
		return 1;
	return i * Factorial(i - 1);
}