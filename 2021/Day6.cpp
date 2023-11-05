#include "stdafx.h"
#include "BigInt.h"

namespace Day6
{
	void Solve()
	{
		cout << "--- Day 6: Lanternfish ---" << endl;
		std::ifstream is("Data6.txt");

		vector<S8> numbers = { 3, 4, 3, 1, 2 };

		/*
			How many lanternfish are gnerated from a single one in X days
			Is there a formula to find out ?
		0	0
		1	6, 8
		2	5, 7
		3	4, 6
			...
		7	0, 2
		8	6, 1, 8
		9	5, 0, 7
		10	4, 6, 6, 8
			...
		14	0, 2, 2, 4
		15	6, 1, 1, 3, 8
		16	5, 0, 0, 2, 7
		17	4, 6, 6, 1, 6, 8, 8
		18	3, 5, 5, 0, 5, 7, 7
		19	2, 4, 4, 6, 4, 6, 6, 8
		20	1, 3, 3, 5, 3, 5, 5, 7
		21	0, 2, 2, 4, 2, 4, 4, 6
		22	6, 1, 1, 3, 1, 3, 3, 5, 8
		23	5, 0, 0, 2, 0, 2, 2, 4, 7
		24	4, 6, 6, 1, 6, 1, 1, 3, 6, 8, 8, 8
		25	3, 5, 5, 0, 5, 0, 0, 2, 5, 7, 7, 7
		26	2, 4, 4, 6, 4, 6, 6, 1, 4, 6, 6, 6, 8, 8, 8
		27	1, 3, 3, 5, 3, 5, 5, 0, 3, 5, 5, 5, 7, 7, 7
		28	0, 2, 2, 4, 2, 4, 4, 6, 2, 4, 4, 4, 6, 6, 6, 8
		29	6, 1, 1, 3, 1, 3, 3, 5, 1, 3, 3, 3, 5, 5, 5, 7, 8
		*/

		const int days = 256;
		U64 fishes[days+1][9] = { 0 };

		char c;
		while (is.get(c))
		{
			fishes[0][c - '0']++;
			is.get(c); // the comma
		}

		/* example
		uGenerated[0][1]	  = 1;
		uGenerated[0][2]	  = 1;
		uGenerated[0][3]	  = 2;
		uGenerated[0][4]	  = 1;
		*/

		U64 sum80 = 0;
		for (int day = 1; day <= days; day++)
		{
			for (int i = 0; i < 8; ++i)
			{
				fishes[day][i] = fishes[day - 1][i + 1];
			}
			fishes[day][6] += fishes[day - 1][0];
			fishes[day][8] = fishes[day - 1][0];

			if (day == 80)
				sum80 = ::accumulate(fishes[day], fishes[day] + 9, 0);
		}

		BigInt sum = 0llu;
		for(int i=0; i < 9; ++i)
			sum += fishes[days][i];


		cout << "Part1: " << sum80 << endl;
		cout << "Part2: " << sum << endl;
	}
} // namespace Day6