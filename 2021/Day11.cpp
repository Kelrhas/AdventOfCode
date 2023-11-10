#include "stdafx.h"

namespace Day11
{
	void Solve()
	{
		cout << "--- Day 11: Dumbo Octopus ---" << endl;
		std::ifstream is("Data11.txt");

		array<array<S8, 10>, 10> grid = {
			5,4,8,3,1,4,3,2,2,3,
			2,7,4,5,8,5,4,7,1,1,
			5,2,6,4,5,5,6,1,7,3,
			6,1,4,1,3,3,6,1,4,6,
			6,3,5,7,3,8,5,4,7,8,
			4,1,6,7,5,2,4,6,4,5,
			2,1,7,6,8,4,1,7,2,1,
			6,8,8,2,8,8,1,1,3,4,
			4,8,4,6,8,4,8,5,5,4,
			5,2,8,3,7,5,1,5,2,6};
		string		   sLine;
		int						 row = 0;
		while (getline(is, sLine))
		{
			for(int i=0; i <10; ++i)
				grid[row][i] = sLine[i] - '0';
			++row;
		}

		auto Display = [&]()
		{
			for (int y = 0; y < grid.size(); ++y)
			{
				for (int x = 0; x < grid[y].size(); ++x)
				{
					cout << (char)(grid[y][x] + '0');
				}
				cout << endl;
			}
		};

		int step = 0;
		U64 uTotalFlash = 0;
		while (++step)
		{
			//cout << "Step " << step;

			// First, the energy level of each octopus increases by 1.
			for (int y = 0; y < grid.size(); ++y)
			{
				for (int x = 0; x < grid[y].size(); ++x)
				{
					grid[y][x]++;
				}
			}

			/*
			* Then, any octopus with an energy level greater than 9 flashes.
			This increases the energy level of all adjacent octopuses by 1,
			including octopuses that are diagonally adjacent.
			If this causes an octopus to have an energy level greater than 9, it also flashes.
			This process continues as long as new octopuses keep having their energy level increased beyond 9.
			(An octopus can only flash at most once per step.)
			*/
			U64 uFlashed = 0;
			while(true)
			{
				bool bHasFlashed = false;
				for (int y = 0; y < grid.size(); ++y)
				{
					for (int x = 0; x < grid[y].size(); ++x)
					{
						if (grid[y][x] > 9)
						{
							++uFlashed;
							bHasFlashed = true;
							for (int y2 = max(0, y - 1); y2 <= y + 1 && y2 < grid.size(); ++y2)
							{
								for (int x2 = max(0, x - 1); x2 <= x + 1 && x2 < grid[y2].size(); ++x2)
								{
									if (grid[y2][x2] >= 0)
										grid[y2][x2]++;
								}
							}
							grid[y][x] = -1;
						}
					}
				}


				if (!bHasFlashed)
					break;
			}

			// Finally, any octopus that flashed during this step has its energy level set to 0, as it used all of its energy to flash.
			for (int y = 0; y < grid.size(); ++y)
			{
				for (int x = 0; x < grid[y].size(); ++x)
				{
					if (grid[y][x] < 0)
						grid[y][x] = 0;
				}
			}

			// just check we did not miss any
			for (int y = 0; y < grid.size(); ++y)
			{
				for (int x = 0; x < grid[y].size(); ++x)
				{
					Assert(grid[y][x] >= 0 && grid[y][x] <= 9);
				}
			}

			//cout << " flashed " << uFlashed << endl;
			//Display();

			if (step <= 100)
				uTotalFlash += uFlashed;

			if (uFlashed == grid.size() * grid[0].size())
				break;
		}

		cout << "Part1: " << uTotalFlash << endl;
		cout << "Part2: " << step << endl;
	}
} // namespace Day11