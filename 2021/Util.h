#pragma once

#include <ctime>
#include <iostream>

#define Assert(exp) {bool b = (exp); if(!b) __debugbreak();}

void DisplayTime(clock_t miliseconds)
{
	if (miliseconds < CLOCKS_PER_SEC)
	{
		cout << miliseconds << "ms";
	}
	else if (miliseconds > CLOCKS_PER_SEC * 60)
	{
		cout << miliseconds / CLOCKS_PER_SEC / 60 << "min";
	}
	else
	{
		cout << miliseconds / CLOCKS_PER_SEC << "sec";
	}
}


template<int N, typename T, glm::qualifier Q>
T ManhatanDist(const glm::vec<N, T, Q>& lhs, const glm::vec<N, T, Q>& rhs)
{
	T dist = 0;
	for (int i = 0; i < N; ++i)
		dist += glm::abs(lhs[i] - rhs[i]);
	return dist;
}