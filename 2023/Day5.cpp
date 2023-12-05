#include "stdafx.h"

namespace Day5
{
	void Solve()
	{
		cout << "--- Day 5: If You Give A Seed A Fertilizer ---" << endl;
		ifstream is("Data5.txt");

		vector<U64> vecSeeds;
		vecSeeds.reserve(32);

		string sLine;
		getline(is, sLine);


		stringstream ss { sLine };
		ss.ignore(231, ':');
		U64 numTmp;
		while (ss >> numTmp)
			vecSeeds.emplace_back(numTmp);

		vector<pair<U64, bool>> vecWorking;
		vecWorking.reserve(vecSeeds.size());
		for (U64 u : vecSeeds)
			vecWorking.emplace_back(u, false);


		struct SeedRange
		{
			U64	 uStart	  = 0;
			U64	 uLength  = 0;
			bool bHandled = false;
		};
		vector<SeedRange> vecRanges;
		vecRanges.reserve(1024); // reserve memory because i bet we gonna have lots of ranges
		for (auto it = vecSeeds.begin(); it != vecSeeds.end(); ++it)
		{
			U64 uStart = *it;
			++it;
			U64 uLength = *it;
			vecRanges.emplace_back(uStart, uLength, false);
		}

		vector<SeedRange> vecToAdd;
		vecToAdd.reserve(1024);
		while (getline(is, sLine))
		{
			if (sLine.empty())
			{
				for (auto &[u, handled] : vecWorking)
					handled = false;

				vecRanges.insert(vecRanges.end(), vecToAdd.begin(), vecToAdd.end());
				vecToAdd.clear();

				for (auto &range : vecRanges)
					range.bHandled = false;
			}
			else if (isdigit(sLine.front()))
			{
				ss = stringstream { sLine };
				U64 uMapStartDest, uMapStartOrig, uMapLength;
				ss >> uMapStartDest >> uMapStartOrig >> uMapLength;

				// Part1
				for (auto &[u, handled] : vecWorking)
				{
					if (!handled)
					{
						if (u >= uMapStartOrig && u <= uMapStartOrig + uMapLength)
						{
							U64 uDist = u - uMapStartOrig;
							u		  = uMapStartDest + uDist;
							handled	  = true;
						}
					}
				}

				// Part 2
				const U64 uMapEndOrig = uMapStartOrig + uMapLength - 1;
				for (auto it = vecRanges.begin(); it != vecRanges.end(); ++it)
				{
					auto &curRange = *it;
					const U64 uBackupLength = curRange.uLength;

					if (!curRange.bHandled)
					{
						const U64 uCurRangeEnd = curRange.uStart + curRange.uLength - 1;
						if (curRange.uStart >= uMapStartOrig && curRange.uStart <= uMapEndOrig)
						{
							// start is somewhere in the range

							const U64 uDist = curRange.uStart - uMapStartOrig;
							if (uCurRangeEnd <= uMapEndOrig)
							{
								// entirely contained -> just modify
								curRange.uStart	  = uMapStartDest + uDist;
								curRange.bHandled = true;
							}
							else
							{
								if (it == vecRanges.begin())
								{
									int a = 10;
								}
								// only start is in the range, we need to split our range in two
								SeedRange left;
								left.uStart	  = uMapStartDest + uDist;
								left.uLength  = uMapEndOrig - curRange.uStart + 1;
								left.bHandled = true;
								vecToAdd.emplace_back(left);

								SeedRange &right = curRange;
								right.uStart   = uMapEndOrig + 1;
								right.uLength  = uCurRangeEnd - (uMapEndOrig + 1) + 1;
								right.bHandled = false;

								Assert(left.uLength > 0 && left.uLength < uBackupLength);
								Assert(right.uLength > 0 && right.uLength < uBackupLength);
								Assert(left.uLength + right.uLength == uBackupLength);
							}
						}
						else if (uCurRangeEnd >= uMapStartOrig && uCurRangeEnd <= uMapEndOrig)
						{
							// end is somewhere in the range, but not start so there is only one case and it's a split
							
							SeedRange &left = curRange;
							left.uStart	  = curRange.uStart;
							left.uLength  = uMapStartOrig - curRange.uStart;
							left.bHandled = false;

							SeedRange right;
							right.uStart   = uMapStartDest;
							right.uLength  = uCurRangeEnd - uMapStartOrig + 1;
							right.bHandled = true;
							vecToAdd.emplace_back(right);

							Assert(left.uLength > 0 && left.uLength < uBackupLength);
							Assert(right.uLength > 0 && right.uLength < uBackupLength);
							Assert(left.uLength + right.uLength == uBackupLength);
						}
						else if (curRange.uStart < uMapStartOrig && uCurRangeEnd > uMapEndOrig)
						{
							// the map is contained inside the range
							// we need to modify a part of the range and create two more ranges

							SeedRange &left = curRange;
							left.uStart		= curRange.uStart;
							left.uLength	= uMapStartOrig - curRange.uStart;
							left.bHandled	= false;

							SeedRange center;
							center.uStart	= uMapStartOrig;
							center.uLength	= uMapLength;
							center.bHandled = true;
							vecToAdd.emplace_back(center);
							
							SeedRange right;
							right.uStart   = uMapEndOrig + 1;
							right.uLength  = uCurRangeEnd - (uMapEndOrig + 1) + 1;
							right.bHandled = false;
							vecRanges.emplace_back(right);

							Assert(left.uLength > 0 && left.uLength < uBackupLength);
							Assert(center.uLength > 0 && center.uLength < uBackupLength);
							Assert(right.uLength > 0 && right.uLength < uBackupLength);
							Assert(left.uLength + center.uLength + right.uLength == uBackupLength);
						}
					}
				}
			}
		}

		auto itMin1 = ranges::min_element(vecWorking, [](auto &lhs, auto &rhs) { return lhs.first < rhs.first; });
		auto itMin2 = ranges::min_element(vecRanges, [](auto &lhs, auto &rhs) { return lhs.uStart < rhs.uStart; });

		cout << "Part1: " << itMin1->first << endl;
		cout << "Part2: " << itMin2->uStart << endl;
	}
} // namespace Day5