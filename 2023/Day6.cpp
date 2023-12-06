#include "stdafx.h"

namespace Day6
{
	void Solve()
	{
		cout << "--- Day 6: Wait For It ---" << endl;
		ifstream is("Data6.txt");

		string sLine;
		int	   temp;

		getline(is, sLine);
		vector<int> vecDurations;
		stringstream ss(sLine.substr(sLine.find_first_of(':') + 1));
		while (ss >> temp)
			vecDurations.emplace_back(temp);

		getline(is, sLine);
		vector<int>	 vecDistances;
		ss = stringstream(sLine.substr(sLine.find_first_of(':') + 1));
		while (ss >> temp)
			vecDistances.emplace_back(temp);


		int sum1 = 1;
		for (S32 i =0; i < vecDurations.size(); ++i)
		{
			const int durationMax = vecDurations[i];
			const int distMax = vecDistances[i];

			int acc = 0;
			for (int d = 0; d < durationMax; ++d)
			{
				int distTravelled = (durationMax - d) * d;
				if (distTravelled > distMax)
				{
					++acc;
				}
			}

			sum1 *= acc;
		}

		const U64 realDuration = std::accumulate(vecDurations.begin(), vecDurations.end(), 0llu, [](U64 sum, U64 n) { return sum * pow(10, ceil(log10(n))) + n; });
		const U64 realDistance = std::accumulate(vecDistances.begin(), vecDistances.end(), 0llu, [](U64 sum, U64 n) { return sum * pow(10, ceil(log10(n))) + n; });

		U64 sum2 = 0;
		for (U64 d = 0; d < realDuration; ++d)
		{
			U64 distTravelled = (realDuration - d) * d;
			if (distTravelled > realDistance)
			{
				++sum2;
			}
		}



		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day6