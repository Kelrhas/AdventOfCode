#include "stdafx.h"

namespace Day3
{
	void Solve()
	{
		cout << "--- Day 3: Binary Diagnostic ---" << endl;
		std::ifstream				  is("Data3.txt");
		std::istream_iterator<string> start(is), end;
		std::vector<string>			  lines(start, end);

		string gamma;
		string epsilon;

		const int NB_CHAR = 12;

		for (int i = 0; i < NB_CHAR; ++i)
		{
			int nbOne = 0;
			for (auto &line : lines)
			{
				if (line[i] == '1')
					nbOne++;
			}

			gamma.push_back(nbOne > lines.size() / 2 ? '1' : '0');
			epsilon.push_back(nbOne > lines.size() / 2 ? '0' : '1');
		}

		int iGamma	 = bitset<NB_CHAR>(gamma).to_ulong();
		int iEpsilon = bitset<NB_CHAR>(epsilon).to_ulong();
		cout << "Part1: " << iGamma << "*" << iEpsilon << "=" << iGamma * iEpsilon << endl;

		vector<string> copy = lines;

		int oxy = 0;
		for (int c = 0; c < NB_CHAR; ++c)
		{
			sort(copy.begin(), copy.end(), [c](const string &lhs, const string &rhs) { return lhs[c] < rhs[c]; });
			for (int i = 0; i < copy.size(); ++i)
			{
				if (copy[i][c] == '1')
				{
					int nbZero = i;
					int nbOne  = copy.size() - nbZero;

					if (nbZero <= nbOne)
					{
						copy.erase(copy.begin(), copy.begin() + nbZero); // remove 0s
						Assert(copy.size() == nbOne);
					}
					else
					{
						copy.erase(copy.begin() + nbZero, copy.end()); // remove 1s
						Assert(copy.size() == nbZero);
					}

					break;
				}
			}
			if (copy.size() == 1)
				break;
		}
		Assert(copy.size() == 1);
		oxy = bitset<NB_CHAR>(copy[0]).to_ulong();

		copy = lines;

		int co2 = 0;
		for (int c = 0; c < NB_CHAR; ++c)
		{
			sort(copy.begin(), copy.end(), [c](const string &lhs, const string &rhs) { return lhs[c] < rhs[c]; });
			for (int i = 0; i < copy.size(); ++i)
			{
				if (copy[i][c] == '1')
				{
					int nbZero = i;
					int nbOne  = copy.size() - nbZero;

					if (nbZero > nbOne)
					{
						copy.erase(copy.begin(), copy.begin() + nbZero); // remove 0s
						Assert(copy.size() == nbOne);
					}
					else
					{
						copy.erase(copy.begin() + nbZero, copy.end()); // remove 1s
						Assert(copy.size() == nbZero);
					}

					break;
				}
			}
			if (copy.size() == 1)
				break;
		}
		Assert(copy.size() == 1);
		co2 = bitset<NB_CHAR>(copy[0]).to_ulong();
		cout << "Part2: " << oxy << "*" << co2 << "=" << oxy * co2 << endl;
	}
} // namespace Day3