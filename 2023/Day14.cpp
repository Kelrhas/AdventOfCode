#include "stdafx.h"

namespace Day14
{

	enum class Cell
	{
		EMPTY = '.',
		WALL  = '#',
		ROCK  = 'O'
	};

	struct Grid
	{
		vector<vector<Cell>> vec;

		bool operator==(const Grid &rhs) const
		{
			const size_t height = vec.size();
			const size_t width	= vec.front().size();
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (vec[y][x] != rhs.vec[y][x])
						return false;
				}
			}
			return true;
		}
	};
} // namespace Day14


template<>
struct hash<Day14::Grid>
{
	size_t operator()(const Day14::Grid &_grid) const
	{
		size_t		 hash	= 0;
		const size_t height = _grid.vec.size();
		const size_t width	= _grid.vec.front().size();
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				hash = (hash << 5) + hash + (size_t)_grid.vec[y][x];
			}
			hash = (hash << 3) + y;
		}
		return hash;
	}
};

namespace Day14
{
	void Solve()
	{
		cout << "--- Day 14: Parabolic Reflector Dish ---" << endl;
		ifstream is("Data14.txt");
		Grid	 grid;

		auto Display = [&grid]()
		{
			for (int y = 0; y < grid.vec.size(); ++y)
			{
				auto &curRow = grid.vec[y];
				for (int x = 0; x < curRow.size(); ++x)
				{
					cout << (unsigned char)curRow[x];
				}
				cout << endl;
			}
		};

		bool	   debug = false;
		glm::ivec2 vPos { 0 };
#if 0
		for (const string &sLine :
			 { "O....#....", "O.OO#....#", ".....##...", "OO.#O....O", ".O.....O#.", "O.#..O.#.#", "..O..#O..O", ".......O..", "#....###..", "#OO..#...." })
#else
		string sLine;
		while (getline(is, sLine))
#endif
		{
			grid.vec.emplace_back();

			vPos.x = 0;
			for (char c : sLine)
			{
				grid.vec.back().emplace_back(Cell { c });
				Assert(grid.vec.back().back() == Cell::EMPTY || grid.vec.back().back() == Cell::WALL || grid.vec.back().back() == Cell::ROCK);

				vPos.x++;
			}

			vPos.y++;
		}

		auto TiltNorth = [&grid]()
		{
			const size_t height = grid.vec.size();
			const size_t width	= grid.vec.front().size();
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					Cell &curCell = grid.vec[y][x];
					if (curCell == Cell::ROCK)
					{
						int yFree = y;
						for (int y2 = y - 1; y2 >= 0; --y2)
						{
							if (grid.vec[y2][x] == Cell::EMPTY)
								yFree = y2;
							else
								break;
						}

						if (yFree != y)
						{
							curCell			   = Cell::EMPTY;
							grid.vec[yFree][x] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltWest = [&grid]()
		{
			const size_t height = grid.vec.size();
			const size_t width	= grid.vec.front().size();

			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					Cell &curCell = grid.vec[y][x];
					if (curCell == Cell::ROCK)
					{
						int xFree = x;
						for (int x2 = x - 1; x2 >= 0; --x2)
						{
							if (grid.vec[y][x2] == Cell::EMPTY)
								xFree = x2;
							else
								break;
						}

						if (xFree != x)
						{
							curCell			   = Cell::EMPTY;
							grid.vec[y][xFree] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltSouth = [&grid]()
		{
			const size_t height = grid.vec.size();
			const size_t width	= grid.vec.front().size();
			for (int y = height - 1; y >= 0; --y)
			{
				for (int x = 0; x < width; ++x)
				{
					Cell &curCell = grid.vec[y][x];
					if (curCell == Cell::ROCK)
					{
						int yFree = y;
						for (int y2 = y + 1; y2 < height; ++y2)
						{
							if (grid.vec[y2][x] == Cell::EMPTY)
								yFree = y2;
							else
								break;
						}

						if (yFree != y)
						{
							curCell			   = Cell::EMPTY;
							grid.vec[yFree][x] = Cell::ROCK;
						}
					}
				}
			}
		};
		auto TiltEast = [&grid]()
		{
			const size_t height = grid.vec.size();
			const size_t width	= grid.vec.front().size();

			for (int x = width - 1; x >= 0; --x)
			{
				for (int y = 0; y < height; ++y)
				{
					Cell &curCell = grid.vec[y][x];
					if (curCell == Cell::ROCK)
					{
						int xFree = x;
						for (int x2 = x + 1; x2 < width; ++x2)
						{
							if (grid.vec[y][x2] == Cell::EMPTY)
								xFree = x2;
							else
								break;
						}

						if (xFree != x)
						{
							curCell			   = Cell::EMPTY;
							grid.vec[y][xFree] = Cell::ROCK;
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
			for (int y = 0; y < grid.vec.size(); ++y)
			{
				auto &curRow = grid.vec[y];
				for (int x = 0; x < curRow.size(); ++x)
				{
					if (curRow[x] == Cell::ROCK)
					{
						uSum += grid.vec.size() - y;
					}
				}
			}
			return uSum;
		};

		unordered_map<Grid, int> hmHashes;
		hmHashes[grid] = 0;

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

		hmHashes[grid] = 1;

		const U64 uMaxCycle = debug ? 100 : 1'000'000'000;
		for (int i = 1; i < uMaxCycle; ++i)
		{
			TiltCycle();
			if (auto it = hmHashes.find(grid); it != hmHashes.end())
			{
				// collision
				if (debug)
				{
					cout << "Cycle found between " << it->second << " and " << i + 1 << endl;
					Display();
				}

				size_t cycleLength	  = i + 1 - it->second;
				size_t remainingCycle = ((uMaxCycle - i) % cycleLength) - 1;
				for (int i2 = 0; i2 < remainingCycle; ++i2)
					TiltCycle();
				break;
			}
			else
				hmHashes.insert({ grid, i + 1 });

			if (debug)
			{
				cout << endl << "After " << i + 1 << " cycles:" << endl;
				Display();
			}
		}
		U64 uSum2 = ComputeLoad();


		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << uSum2 << endl;
	}
} // namespace Day14