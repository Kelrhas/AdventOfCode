#include "stdafx.h"

namespace Day14
{
	void Solve()
	{
		cout << "--- Day 14: Parabolic Reflector Dish ---" << endl;
		ifstream is("Data14.txt");

		enum class Cell
		{
			EMPTY = '.',
			WALL  = '#',
			ROCK  = 'O'
		};

		vector<vector<Cell>> grid;

		auto Display = [&grid]()
		{
			for (int y = 0; y < grid.size(); ++y)
			{
				auto &curRow = grid[y];
				for (int x = 0; x < curRow.size(); ++x)
				{
					cout << (unsigned char)curRow[x];
				}
				cout << endl;
			}
		};

		bool	   debug = false;
		glm::ivec2 vPos { 0 };
#if 1
		for (const string &sLine :
			 { "O....#....", "O.OO#....#", ".....##...", "OO.#O....O", ".O.....O#.", "O.#..O.#.#", "..O..#O..O", ".......O..", "#....###..", "#OO..#...." })
#else
		string	   sLine;
		while (getline(is, sLine))
#endif
		{
			grid.emplace_back();

			vPos.x = 0;
			for (char c : sLine)
			{
				grid.back().emplace_back(Cell { c });
				Assert(grid.back().back() == Cell::EMPTY || grid.back().back() == Cell::WALL || grid.back().back() == Cell::ROCK);

				vPos.x++;
			}

			vPos.y++;
		}

		auto TiltNorth = [&grid]()
		{
			const size_t height = grid.size();
			const size_t width	= grid.front().size();
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					Cell &curCell = grid[y][x];
					if (curCell == Cell::ROCK)
					{
						int yFree = y;
						for (int y2 = y - 1; y2 >= 0; --y2)
						{
							if (grid[y2][x] == Cell::EMPTY)
								yFree = y2;
							else
								break;
						}

						if (yFree != y)
						{
							curCell		   = Cell::EMPTY;
							grid[yFree][x] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltWest = [&grid]()
		{
			const size_t height = grid.size();
			const size_t width	= grid.front().size();

			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					Cell &curCell = grid[y][x];
					if (curCell == Cell::ROCK)
					{
						int xFree = x;
						for (int x2 = x - 1; x2 >= 0; --x2)
						{
							if (grid[y][x2] == Cell::EMPTY)
								xFree = x2;
							else
								break;
						}

						if (xFree != x)
						{
							curCell		   = Cell::EMPTY;
							grid[y][xFree] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltSouth = [&grid]()
		{
			const size_t height = grid.size();
			const size_t width	= grid.front().size();
			for (int y = height - 1; y >= 0; --y)
			{
				for (int x = 0; x < width; ++x)
				{
					Cell &curCell = grid[y][x];
					if (curCell == Cell::ROCK)
					{
						int yFree = y;
						for (int y2 = y + 1; y2 < height; ++y2)
						{
							if (grid[y2][x] == Cell::EMPTY)
								yFree = y2;
							else
								break;
						}

						if (yFree != y)
						{
							curCell		   = Cell::EMPTY;
							grid[yFree][x] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltEast = [&grid]()
		{
			const size_t height = grid.size();
			const size_t width	= grid.front().size();

			for (int x = width - 1; x >= 0; --x)
			{
				for (int y = 0; y < height; ++y)
				{
					Cell &curCell = grid[y][x];
					if (curCell == Cell::ROCK)
					{
						int xFree = x;
						for (int x2 = x + 1; x2 < width; ++x2)
						{
							if (grid[y][x2] == Cell::EMPTY)
								xFree = x2;
							else
								break;
						}

						if (xFree != x)
						{
							curCell		   = Cell::EMPTY;
							grid[y][xFree] = Cell::ROCK;
						}
					}
				}
			}
		};

		auto TiltCycle = [&]()
		{
			TiltNorth();
			TiltWest();
			TiltSouth();
			TiltEast();
		};

		auto ComputeLoad = [&grid]() -> U64
		{
			U64 uSum = 0;
			for (int y = 0; y < grid.size(); ++y)
			{
				auto &curRow = grid[y];
				for (int x = 0; x < curRow.size(); ++x)
				{
					if (curRow[x] == Cell::ROCK)
					{
						uSum += grid.size() - y;
					}
				}
			}
			return uSum;
		};


		TiltNorth();
		U64 uSum1 = ComputeLoad();
		if (debug)
		{
			cout << endl << "After north:" << endl;
			Display();
		}
		// finish first cycle
		TiltWest();
		if (debug)
		{
			cout << endl << "After west:" << endl;
			Display();
		}
		TiltSouth();
		if (debug)
		{
			cout << endl << "After south:" << endl;
			Display();
		}
		TiltEast();
		if (debug)
		{
			cout << endl << "After east:" << endl;
			Display();
		}

#error Need cache to sort out rocks that do not move or something like that, and also find if at one point the rocks stop moving altogether between cycles
		
		const U64 uMaxCycle = debug ? 3 : 1'000'000'000;
		for (int i = 1; i < uMaxCycle; ++i)
		{
			TiltCycle();
			if (debug)
			{
				cout << "After " << i + 1 << " cycles:" << endl;
				Display();
			}
		}
		U64 uSum2 = ComputeLoad();


		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << uSum2 << endl;
	}
} // namespace Day14