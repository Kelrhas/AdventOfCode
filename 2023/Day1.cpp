#include "stdafx.h"

namespace Day1
{
	void Solve()
	{
		cout << "--- Day 1:  ---" << endl;
		ifstream is("Data1.txt");

		int sum1 = 0;
		int sum2 = 0;
		string		sLine;
		while (getline(is, sLine))
		{
			int leftmostVal =0;
			int rightmostVal =0;
			int leftmostIndex = sLine.size();
			int rightmostIndex = -1;
			{
				int index = 0;
				for (char c : sLine)
				{
					if (isdigit(c))
					{
						leftmostVal = (c - '0');
						sum1 += leftmostVal * 10;
						leftmostIndex = index;
						break;
					}
					index++;
				}
				index = sLine.size()-1;
				for (char c : sLine | views::reverse)
				{
					if (isdigit(c))
					{
						rightmostVal = (c - '0');
						sum1 += rightmostVal;
						rightmostIndex = index;
						break;
					}
					index--;
				}
			}

			constexpr const char *NUMS[] { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
			for (int i = 1; i < 10; ++i)
			{
				auto   pNum = NUMS[i];
				int ind	= sLine.find(pNum);
				if (ind != string::npos && ind < leftmostIndex)
				{
					leftmostIndex = ind;
					leftmostVal	  = i;
				}

				ind = sLine.rfind(pNum);
				if (ind != string::npos && ind > rightmostIndex)
				{
					rightmostIndex = ind;
					rightmostVal   = i;
				}
			}
			sum2 += leftmostVal * 10 + rightmostVal;

		}

		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day1