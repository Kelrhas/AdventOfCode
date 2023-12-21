#include "stdafx.h"

namespace Day21
{
	void Solve()
	{
		cout << "--- Day 21: Step Counter ---" << endl;

		unordered_set<glm::ivec2> rocks;
		glm::ivec2				  vStart, vSize { 0 };

#if 0
		for (string sLine : {
			   "...........",
			   ".....###.#.",
			   ".###.##..#.",
			   "..#.#...#..",
			   "....#.#....",
			   ".##..S####.",
			   ".##..#...#.",
			   ".......##..",
			   ".##.#.####.",
			   ".##..##.##.",
			   "...........",
			 })
#else
		ifstream is("Data21.txt");
		string	 sLine;
		while (getline(is, sLine))
#endif
		{
			vSize.x = 0;
			for (char c : sLine)
			{
				if (c == 'S')
					vStart = vSize;
				if (c == '#')
					rocks.insert(vSize);
				vSize.x++;
			}

			vSize.y++;
		}

		bool						   debug = false;
		unordered_set<glm::ivec2>	   setPos, setSwap;
		unordered_map<glm::ivec2, U32> cache; // store the step for parity/oddness
		setPos.reserve(1 << 20);
		setSwap.reserve(1 << 20);
		cache.reserve(1 << 20);


		auto Process = [&](bool bInfiniteMap, U32 nbSteps)
		{
			setPos.clear();
			setSwap.clear();
			cache.clear();

			setPos.insert(vStart);
			for (U32 i = 0; i < nbSteps; ++i)
			{
				setSwap.clear();
				for (const auto &vPos : setPos)
				{
					cache.insert({ vPos, i });

					auto Insert = [&](const glm::ivec2 &v)
					{
						if (auto it = cache.find(v); it != cache.end())
							return;

						glm::ivec2 vForRock = v;
						if (bInfiniteMap)
						{
							while (vForRock.x < 0)
							{
								vForRock.x += vSize.x;
							}
							while (vForRock.y < 0)
							{
								vForRock.y += vSize.y;
							}
							vForRock.x = vForRock.x % vSize.x;
							vForRock.y = vForRock.y % vSize.y;
						}
						if (rocks.find(vForRock) == rocks.end())
							setSwap.insert(v);
					};
					glm::ivec2 vLeft = vPos + glm::ivec2 { -1, 0 };
					if (bInfiniteMap || vLeft.x >= 0)
						Insert(vLeft);
					glm::ivec2 vRight = vPos + glm::ivec2 { 1, 0 };
					if (bInfiniteMap || vRight.x < vSize.x)
						Insert(vRight);
					glm::ivec2 vUp = vPos + glm::ivec2 { 0, -1 };
					if (bInfiniteMap || vUp.y >= 0)
						Insert(vUp);
					glm::ivec2 vDown = vPos + glm::ivec2 { 0, 1 };
					if (bInfiniteMap || vDown.y < vSize.y)
						Insert(vDown);
				}

				swap(setPos, setSwap);

				if (debug && !bInfiniteMap)
				{
					cout << "Step " << i + 1 << endl;
					for (int y = 0; y < vSize.y; ++y)
					{
						for (int x = 0; x < vSize.x; ++x)
						{
							char	   c = '.';
							glm::ivec2 v { x, y };
							if (auto it = cache.find(v); it != cache.end())
							{
								c = '0' + it->second %2;
								SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
							}
							else if (setPos.find(v) != setPos.end())
							{
								c = 'O';
								SetConsoleTextAttribute(hConsole, CONSOLE_CYAN);
							}
							else if (rocks.find(v) != rocks.end())
							{
								c = '#';
								SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
							}
							cout << c;
							SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
						}
						cout << endl;
					}
					cout << endl;
				}
			}

			for (const auto &v : setPos)
				cache.insert({ v, nbSteps });

			return count_if(cache.begin(), cache.end(), [&](const auto &pair) { return pair.second % 2 == nbSteps % 2; });
		};

		// Process(false, debug ? 6 : 64);
		cout << "Part1: " << Process(false, 6) << "/" << cache.size() << endl;

		// Process(true, debug ? 6 : 26501365); 
		cout << "Part2: " << Process(true, 26501365) << endl;
	}
} // namespace Day21