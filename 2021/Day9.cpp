#include "stdafx.h"

namespace Day9
{
	void Solve()
	{
		cout << "--- Day 9: Smoke Basin ---" << endl;
		std::ifstream is("Data9.txt");

		vector<string> grid;
		string		   sLine;
		while (getline(is, sLine))
		{
			grid.emplace_back(sLine);
		}

		//grid.clear();
		//grid.emplace_back("2199943210");
		//grid.emplace_back("3987894921");
		//grid.emplace_back("9856789892");
		//grid.emplace_back("8767896789");
		//grid.emplace_back("9899965678");

		unordered_map<glm::ivec2, glm::ivec2> cache;

		vector<glm::ivec2> vecDescent;
		size_t			   columnSize = grid.size();
		U64				   sum		  = 0;
		for (int y = 0; y < columnSize; ++y)
		{
			size_t rowSize = grid[y].size();

			for (int x = 0; x < rowSize; ++x)
			{
				glm::ivec2 vCoord(x, y);
				if (cache.find(vCoord) != cache.end())
					continue;

				// descend to find the lowest from here
				vecDescent.clear();

				while (true)
				{
					const char val	 = grid[vCoord.y][vCoord.x];
					const char left	 = vCoord.x > 0 ? grid[vCoord.y][vCoord.x - 1] : '9';
					const char right = vCoord.x < rowSize - 1 ? grid[vCoord.y][vCoord.x + 1] : '9';
					const char up	 = vCoord.y > 0 ? grid[vCoord.y - 1][vCoord.x] : '9';
					const char down	 = vCoord.y < columnSize - 1 ? grid[vCoord.y + 1][vCoord.x] : '9';

					vecDescent.emplace_back(vCoord);

					if (val == 0 || (left > val && right > val && up > val && down > val))
					{
						// end of the line
						if (cache.find(vCoord) == cache.end())
							sum += val - '0' + 1;
						for (const auto &v : vecDescent)
							cache[v] = vCoord;
						break;
					}
					else
					{
						if (vCoord.x > 0 && left < right && left < up && left < down)
							vCoord.x--;
						else if (vCoord.x < rowSize - 1 && right < up && right < down)
							vCoord.x++;
						else if (vCoord.y > 0 && up < down)
							vCoord.y--;
						else if (vCoord.y < columnSize - 1)
							vCoord.y++;
						else // out of grid
						{
							if (cache.find(vCoord) == cache.end())
								sum += val - '0' + 1;
							for (const auto &v : vecDescent)
								cache[v] = vCoord;
							break;
						}
					}
				}
			}
		}

		cout << "Part1: " << sum << endl;


		unordered_map<glm::ivec2, int> counts;
		for (const auto &[vCoord, vLow] : cache)
		{
			if (grid[vCoord.y][vCoord.x] < '9')
				counts[vLow]++;
		}


		// sad try to use ranges, almost working except for partial_sort :(
		//auto view = ranges::partial_sort(counts.begin(), counts.begin() + 3, counts.end(), greater<> {}, [](auto &pair) { return pair.second; }) | views::values | views::take(3) | views::common;
		//U64	 product = std::accumulate(view.begin(), view.end(), 1, multiplies<> {});

		auto		view = counts | views::values;
		vector<int> sizes(view.begin(), view.end());
		partial_sort(sizes.begin(), sizes.begin() + 3, sizes.end(), greater {});


		cout << "Part2: " << sizes[0] * sizes[1] * sizes[2] << endl;
	}
} // namespace Day9