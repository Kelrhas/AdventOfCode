#include "stdafx.h"

namespace Day2
{
	void Solve()
	{
		cout << "--- Day 2: Cube Conundrum ---" << endl;
		ifstream is("Data2.txt");

		string sLine = "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green";
		U32	   sum1	 = 0;
		U32	   sum2	 = 0;
		while (getline(is, sLine))
		{
			auto		aze = sLine.find_first_of(':');
			string_view svRemaining(sLine.begin() + 5, sLine.begin() + sLine.find_first_of(':'));
			int			gameNumber;
			from_chars(svRemaining.data(), svRemaining.data() + svRemaining.size(), gameNumber);

			svRemaining = string_view(sLine.begin() + svRemaining.size() +6, sLine.end());
			
			int iMaxRed = 0;
			int iMaxGreen = 0;
			int iMaxBlue = 0;

			while (!svRemaining.empty())
			{
				string_view svSubset = string_view(svRemaining.begin(), svRemaining.begin() + min(svRemaining.find_first_of(';'), svRemaining.length()));
				if (svRemaining.size() > svSubset.size())
					svRemaining = string_view(svRemaining.begin() + svSubset.size() + 1, svRemaining.end());
				else
					svRemaining = {};
				for (const auto &cubes : svSubset | views::split(','))
				{
					string_view svCube { cubes.begin(), cubes.end() };

					int nbCube			 = 0;
					string_view svNum { svCube.data() + svCube.find_first_of("0123456789"), svCube.data() + svCube.find_last_of("0123456789")+1 };
					from_chars(svNum.data(), svNum.data() + svNum.size(), nbCube);
					if (svCube.ends_with('d'))
					{
						iMaxRed = max(nbCube, iMaxRed);
					}
					else if (svCube.ends_with('n'))
					{
						iMaxGreen = max(nbCube, iMaxGreen);
					}
					else if (svCube.ends_with('e'))
					{
						iMaxBlue = max(nbCube, iMaxBlue);
					}
				}
			}

			if (iMaxRed <= 12 && iMaxGreen <= 13 && iMaxBlue <= 14)
				sum1 += gameNumber;

			sum2 += iMaxRed * iMaxGreen * iMaxBlue;
		}

		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day2