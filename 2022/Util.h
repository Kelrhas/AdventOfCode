#pragma once

#define Assert(exp) {bool b = (exp); if(!b) __debugbreak();}

void DisplayTime(double seconds);

template<typename T>
struct powers
{
	constexpr T operator()(T const &lhs, T const &rhs) const { return (T)std::pow(lhs, rhs); }
};