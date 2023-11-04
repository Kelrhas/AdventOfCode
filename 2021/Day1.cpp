#include "stdafx.h"

namespace Day1
{
	void Solve()
	{
		cout << "--- Day 1: Sonar Sweep ---" << endl;
		std::ifstream			   is("Data1.txt");
		std::istream_iterator<int> start(is), end;
		std::vector<int>		   numbers(start, end);

		int sum = 0;
		for (int i = 1; i < numbers.size(); ++i)
		{
			if (numbers[i] > numbers[i - 1])
				sum++;
		}

		cout << "Part1:" << sum << endl;

		sum		 = 0;
		int last = numbers[2] + numbers[1] + numbers[0];
		;
		for (int i = 3; i < numbers.size(); ++i)
		{
			int window = numbers[i] + numbers[i - 1] + numbers[i - 2];
			if (window > last)
				sum++;
			last = window;
		}

		cout << "Part2:" << sum << endl;
	}
} // namespace Day1