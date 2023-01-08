#include "stdafx.h"

namespace Day1
{
	void Solve()
	{
		cout << "--- Day 1: Calorie Counting ---" << endl;
		std::ifstream	 is("Data1.txt");
		std::vector<int> elves({ 0 });

		string line;
		while (getline(is, line))
		{
			if (line.empty())
			{
				elves.push_back(0);
			}
			else
			{
				int cal = atoi(line.c_str());
				elves.back() += cal;
			}
		}

		std::sort(elves.begin(), elves.end(), std::greater<int>());

		cout << "Part1:" << elves.front() << endl;
		cout << "Part2:" << (elves[0] + elves[1] + elves[2]) << endl;
	}
} // namespace Day1