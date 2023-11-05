#include "stdafx.h"

namespace Day7
{
	void Solve()
	{
		cout << "--- Day 7: The Treachery of Whales ---" << endl;
		std::ifstream is("Data7.txt");

		string sLine;
		is >> sLine;

		/* example
		sLine = "16,1,2,0,4,2,7,1,2,14";
		/**/
		vector<int> vecNumbers;
		for (const auto& sub : sLine | views::split(','))
		{
			string s(sub.begin(), sub.end());
			vecNumbers.emplace_back(stoi(s));
		}

		sort(vecNumbers.begin(), vecNumbers.end());
		S64 median = vecNumbers[vecNumbers.size() / 2];
		S64 fuelSimple = 0;
		for (int n : vecNumbers)
		{
			int dist = abs(n - median);
			fuelSimple += dist;
		}

		cout << "Part1: " << fuelSimple << endl;

		U64 uBestFuel = -1;
		for (int h = vecNumbers.front(); h < vecNumbers.back(); ++h)
		{
			U64 fuelExpo = 0;
			for (int n : vecNumbers)
			{
				int dist = abs(n - h);
				fuelExpo += dist * (dist + 1) / 2;
				if (fuelExpo >= uBestFuel)
					break;
			}

			uBestFuel = min(uBestFuel, fuelExpo);
		}
		cout << "Part2: " << uBestFuel << endl;
	}
} // namespace Day7