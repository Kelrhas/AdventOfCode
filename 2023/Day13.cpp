#include "stdafx.h"

namespace Day13
{
	struct Pattern
	{
		// optim idea: use bitfield for fast comparison
		vector<string> horizontal;
		vector<string> vertical; // for easy comparison

		void Display(bool bHorizontal) const
		{
			int iLine = 0;
			if (bHorizontal)
			{
				cout << "Horizontal" << endl;

				for (const string &line : horizontal)
				{
					cout << iLine++ << '\t' << line << endl;
				}
			}
			else
			{
				cout << "Vertical" << endl;
				for (const string &line : vertical)
				{
					cout << iLine++ << '\t' << line << endl;
				}
			}
		}
	};

	void Solve()
	{
		cout << "--- Day 13: Point of Incidence ---" << endl;
		ifstream is("Data13.txt");

		vector<Pattern> patterns;


		const bool debug = false;

		U32	   uSum1 = 0;
		string sLine;
		patterns.push_back({}); // first pattern
		while (getline(is, sLine))
		{
			if (sLine.empty())
			{
				patterns.push_back({}); // next pattern
				continue;
			}

			patterns.back().horizontal.emplace_back(sLine);
		}

		// compute vertical strings
		for (Pattern &pattern : patterns)
		{
			size_t width  = pattern.horizontal.front().size();
			size_t height = pattern.horizontal.size();
			for (int x = 0; x < width; ++x)
			{
				string s(height, '0');
				for (int y = 0; y < height; ++y)
				{
					s[y] = pattern.horizontal[y][x];
				}
				pattern.vertical.emplace_back(s);
			}
		}

		// compute each pattern
		for (Pattern &pattern : patterns)
		{
			// find lines that match somewhere down the pattern
			auto FindSymmetry = [](const vector<string>& vec) ->int
			{
				const size_t height = vec.size();
				for (int iFirst = 0; iFirst < height; ++iFirst)
				{
					const string &sFirst = vec[iFirst];
					for (int iMatch = iFirst + 1; iMatch < height; ++iMatch)
					{
						// no symmetry if odd number of line between them
						if ((iMatch - iFirst) % 2 == 0)
							continue;

						const string &sMatch = vec[iMatch];

						if (sFirst.compare(sMatch) == 0)
						{
							// early out if there are remaining strings that should have been caught by the first pass
							if (iFirst > 0 && iMatch < height - 1)
								break;

							// both are equals, we need to check if all string inbetween are equal
							const int iHalfSize = (iMatch - iFirst + 1) / 2;
							bool	  bGood		= true;
							for (int i = 1; i < iHalfSize && bGood; ++i)
							{
								const string &sAfterFirst  = vec[iFirst + i];
								const string &sBeforeMatch = vec[iMatch - i];
								bGood					   = (sAfterFirst.compare(sBeforeMatch) == 0);
							}

							if (bGood)
							{
								if (debug)
								{
									cout << "Found symmetry on line " << iFirst << " with line " << iMatch << endl;
								}
								return floorf((iFirst + iMatch)/2) +1 ;
							}
						}
					}
				}

				return -1;
			};

			if (debug)
				pattern.Display(true);
			int iLineH = FindSymmetry(pattern.horizontal);
			if (iLineH >= 0)
				uSum1 += 100 * iLineH;

			if (debug)
			{
				cout << endl;
				pattern.Display(false);
			}
			int iLineV = FindSymmetry(pattern.vertical);
			if (iLineV >= 0)
				uSum1 += iLineV;

			Assert(iLineH >= 0 || iLineV >= 0);

			if (debug) // only one pattern
				break;
		}
		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << endl;
	}
} // namespace Day13