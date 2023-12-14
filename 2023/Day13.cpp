#include "stdafx.h"

namespace Day13
{
	struct Pattern
	{
		vector<string> sHorizontal;
		vector<string> sVertical; // for easy comparison
		// use bitfield for fast comparison
		vector<U64> uHorizontal { 0 }; // inverted from string, LSB if first char !
		vector<U64> uVertical { 0 };

		U32 score1 = 0;
		U32 score2 = 0;

		void Display(bool bHorizontal) const
		{
			int iLine = 0;
			if (bHorizontal)
			{
				cout << "Horizontal:" << endl;
				for (int i = 0; i < sHorizontal.size(); ++i)
				{
					const string &sLine = sHorizontal[i];
					const string  uLine = bitset<32>(uHorizontal[i]).to_string().substr(32 - sLine.size());
					cout << iLine++ << '\t' << sLine << " -> " << uLine << endl;
				}
			}
			else
			{
				cout << "Vertical:" << endl;
				for (int i = 0; i < sVertical.size(); ++i)
				{
					const string &sLine = sVertical[i];
					const string  uLine = bitset<32>(uVertical[i]).to_string().substr(32 - sLine.size());
					cout << iLine++ << '\t' << sLine << " -> " << uLine << endl;
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

		U32	   uSum1 = 0, uSum2 = 0;
		string sLine;
		patterns.push_back({}); // first pattern
		while (getline(is, sLine))
		{
			if (sLine.empty())
			{
				patterns.push_back({}); // next pattern
				continue;
			}

			patterns.back().sHorizontal.emplace_back(sLine);
		}

		// compute vertical strings & bitfields
		for (Pattern &pattern : patterns)
		{
			size_t width  = pattern.sHorizontal.front().size();
			size_t height = pattern.sHorizontal.size();
			for (int x = 0; x < width; ++x)
			{
				string s(height, '0');
				for (int y = 0; y < height; ++y)
				{
					s[y] = pattern.sHorizontal[y][x];
				}
				pattern.sVertical.emplace_back(s);
			}

			pattern.uHorizontal.resize(height);
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (pattern.sHorizontal[y][x] == '#')
						pattern.uHorizontal[y] |= 1 << x;
				}
			}

			pattern.uVertical.resize(width);
			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					if (pattern.sVertical[x][y] == '#')
						pattern.uVertical[x] |= 1 << y;
				}
			}
		}

		// compute each pattern
		for (Pattern &pattern : patterns)
		{
			// find lines that match somewhere down the pattern
			auto FindSymmetryBitfield = [](const vector<U64> &vec, int scoreMul, int diff) -> int
			{
				const size_t height = vec.size();
				for (int iFirst = 0; iFirst < height; ++iFirst)
				{
					const U64 &sFirst = vec[iFirst];
					for (int iMatch = iFirst + 1; iMatch < height; ++iMatch)
					{
						// no symmetry if odd number of line between them
						if ((iMatch - iFirst) % 2 == 0)
							continue;

						const U64 &sMatch = vec[iMatch];

						if (sFirst == sMatch)
						{
							// early out if there are remaining strings that should have been caught by the first pass
							if (iFirst > 0 && iMatch < height - 1)
								break;

							// both are equals, we need to check if all line inbetween are equal
							const int iHalfSize = (iMatch - iFirst + 1) / 2;
							bool	  bGood		= true;
							for (int i = 1; i < iHalfSize && bGood; ++i)
							{
								const U64 &sAfterFirst	= vec[iFirst + i];
								const U64 &sBeforeMatch = vec[iMatch - i];
								bGood					= (sAfterFirst == sBeforeMatch);
							}

							if (bGood)
							{
								int score = (floorf((iFirst + iMatch) / 2) + 1) * scoreMul;
								if (score != diff)
								{
									if (debug)
									{
										cout << "Found symmetry on line " << iFirst << " with line " << iMatch << endl;
									}
									return score;
								}
							}
						}
					}
				}

				return -1;
			};

			// Part 1
			{
				if (debug)
					pattern.Display(true);
				int scoreH = FindSymmetryBitfield(pattern.uHorizontal, 100, 0);
				if (scoreH >= 0)
					pattern.score1 = scoreH;

				if (debug)
				{
					cout << endl;
					pattern.Display(false);
				}
				int scoreV = FindSymmetryBitfield(pattern.uVertical, 1, 0);
				if (scoreV >= 0)
					pattern.score1 = scoreV;


				Assert(scoreH >= 0 || scoreV >= 0); // cannot be both ?
				Assert(pattern.score1 != 0);
				uSum1 += pattern.score1;
			}

			// for part 2, we need to change exactly one bit anywhere in the pattern and still find symmetry (but not the same)
			bool		foundSymmetry = false;
			vector<U64> vecHori		  = pattern.uHorizontal;
			vector<U64> vecVert		  = pattern.uVertical;
			for (int y = 0; y < pattern.sHorizontal.size() && !foundSymmetry; ++y)
			{
				for (int x = 0; x < pattern.sVertical.size() && !foundSymmetry; ++x)
				{
					vecHori[y] ^= 1 << x; // change one bit
					int scoreH = FindSymmetryBitfield(vecHori, 100, pattern.score1);
					if (scoreH >= 0) // found
					{
						if (scoreH != pattern.score1)
						{
							pattern.score2 = scoreH;
							foundSymmetry  = true;
							if (debug)
							{
								cout << "Found hori " << scoreH << " from changing bit " << x << "," << y << endl;

								for (int y2 = 0; y2 < pattern.sHorizontal.size(); ++y2)
								{
									const string uLine = bitset<32>(vecHori[y2]).to_string().substr(32 - pattern.sHorizontal[y2].size());
									cout << pattern.sHorizontal[y2] << " -> " << uLine << endl;
								}
							}
							break;
						}
					}
					vecHori[y] ^= 1 << x; // revert the bit

					vecVert[x] ^= 1 << y; // change one bit
					int scoreV = FindSymmetryBitfield(vecVert, 1, pattern.score1);
					if (scoreV >= 0) // found
					{
						U32 score = scoreV;
						if (score != pattern.score1)
						{
							pattern.score2 = score;
							foundSymmetry = true;
							if (debug)
							{
								cout << "Found vert " << scoreV << " from changing bit " << x << "," << y << endl;

								for (int y2 = 0; y2 < pattern.sVertical.size(); ++y2)
								{
									const string uLine = bitset<32>(vecVert[y2]).to_string().substr(32 - pattern.sVertical[y2].size());
									cout << pattern.sVertical[y2] << " -> " << uLine << endl;
								}
							}
							break;
						}
					}
					vecVert[x] ^= 1 << y; // revert the bit
				}
			}

			Assert(pattern.score2 != 0);
			uSum2 += pattern.score2;

			if (debug) // only one pattern
				break;
		}
		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << uSum2 << endl;
	}
} // namespace Day13