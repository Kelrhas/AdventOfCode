#include "stdafx.h"

namespace Day11
{
	void Solve()
	{
		cout << "--- Day 11: Cosmic Expansion ---" << endl;
		ifstream is("Data11.txt");

		vector<glm::ivec2> galaxies;
		vector<bool>	   columnsTest; // to keep track of galaxies position
		vector<int>		   rows;		// contains rows that do not have galaxies
		string			   sLine;
		int				   y = 0;
		while (getline(is, sLine))
		{
			columnsTest.resize(sLine.size());

			bool bGalaxy = false;
			int	 x		 = 0;
			for (char c : sLine)
			{
				if (c == '#')
				{
					galaxies.emplace_back(x, y);
					columnsTest[x] = true;
					bGalaxy = true;
				}
				x++;
			}

			if (!bGalaxy)
				rows.push_back(y);
			y++;
		}


		int			x = 0;
		vector<int> columns; // contains columns that do not have galaxies
		for (bool b : columnsTest)
		{
			if (!b)
				columns.emplace_back(x);
			x++;
		}

		U32		  sum1		 = 0;
		U64		  sum2		 = 0;
		const int nbGalaxies = galaxies.size();
		for (int i = 0; i < nbGalaxies; ++i)
		{
			for (int j = i+1; j < nbGalaxies; ++j)
			{
				const glm::ivec2& vPos1 = galaxies[i];
				const glm::ivec2& vPos2 = galaxies[j];

				U64 baseDist = ManhattanDist(vPos1, vPos2);
				U64 dist2	 = baseDist;

				// add each row and columns that needs to be doubled
				{
					const glm::ivec2 &vStart = (vPos1.y <= vPos2.y ? vPos1 : vPos2);
					const glm::ivec2 &vEnd	 = (vPos1.y <= vPos2.y ? vPos2 : vPos1);
					for (int row : rows)
					{
						if (row > vEnd.y)
							break;
						if (row > vStart.y)
						{
							baseDist++;
							dist2 += 999'999;
						}
					}
				}
				{
					const glm::ivec2 &vStart = (vPos1.x <= vPos2.x ? vPos1 : vPos2);
					const glm::ivec2 &vEnd   = (vPos1.x <= vPos2.x ? vPos2 : vPos1);
					for (int col : columns)
					{
						if (col > vEnd.x)
							break;
						if (col > vStart.x)
						{
							baseDist++;
							dist2 += 999'999;
						}
					}
				}

				sum1 += baseDist;
				sum2 += dist2;
			}
		}


		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day11