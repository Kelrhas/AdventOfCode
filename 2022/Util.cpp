#include "stdafx.h"

#include <iostream>

void DisplayTime(double seconds)
{
	if (seconds > 100)
	{
		std::cout << seconds / 60 << " min";
	}
	else if (seconds > 1)
	{
		std::cout << seconds << " sec";
	}
	else if(seconds > 0.001)
	{
		std::cout << seconds * 1000 << " ms";
	}
	else
	{
		std::cout << seconds * 1'000'000 << " µs";
	}
}
