#include "stdafx.h"

namespace Day17
{
	enum Shape
	{
		HORIZONTAL,
		CROSS,
		L,
		VERTICAL,
		CUBE,

		COUNT
	};

	struct Rock
	{
		int				   width;
		int				   height;
		Shape			   shape;
		vector<glm::ivec2> pos;
	};


	Rock rocks[Shape::COUNT] = {
		{ 4, 1, HORIZONTAL, { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } } },  { 3, 3, CROSS, { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } } },
		{ 3, 3, L, { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 }, { 2, 2 } } }, { 1, 4, VERTICAL, { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } } },
		{ 2, 2, CUBE, { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } },
	};

	struct State
	{
		Shape		  shape;
		int			  move;
		array<int, 7> heightmap;


		bool operator==(State const &rhs) const { return shape == rhs.shape && move == rhs.move && heightmap == rhs.heightmap; }
	};
} // namespace Day17

template<>
struct std::hash<Day17::State>
{
	auto operator()(const Day17::State &s) const
	{
		size_t const h1 = (size_t)s.shape;
		size_t const h2 = s.move;
		size_t		 h3 = 19;
		for (size_t i = 0; i < s.heightmap.size(); ++i)
		{
			h3 = (h3 << i) ^ (s.heightmap[i]);
		}
		return ((h1 ^ (h2 << 1)) ^ (h3 << 3));
	}
};

namespace Day17
{
	void Solve()
	{
		cout << "--- Day 17: Pyroclastic Flow ---" << endl;
		ifstream is("Data17.txt");
		string	 line;
		getline(is, line);


		size_t nbDirection = line.size();
		size_t curDir	   = 0;
		Shape  curShape	   = HORIZONTAL;


		vector<bitset<7>> grid; // dynamic growth
		grid.reserve(5000);

		vector<size_t> heights;
		size_t		   maxHeight  = 0;
		bool		   debug	  = false;
		const size_t   enoughStep = lcm(nbDirection, (size_t)Shape::COUNT) * 1000;

		unordered_map<State, int> cache;


		size_t part1 = 0;
		size_t part2 = 0;
		for (size_t step = 1; step <= enoughStep; ++step)
		{
			// place rock at the correct location
			// iterate until at rest
			Rock &curRock = rocks[curShape];
			// position is the bottom left corner !! not the top
			int	   posX = 2;
			size_t posY = maxHeight + 3;
			if (posY + curRock.height >= grid.size())
				grid.resize(posY + curRock.height + 1);

			if (debug)
				cout << "Next rock" << endl;

			auto Display = [&](size_t bottom = 0, size_t top = -1) {
				if (top == -1)
					top = grid.size() - 1;

				for (size_t y = top; y >= bottom && y < grid.size(); --y)
				{
					cout << '|';
					for (int i = 0; i < grid[y].size(); ++i)
					{
						char c = '.';
						for (const auto &pos : curRock.pos)
						{
							if (posX + pos.x == i && posY + pos.y == y)
							{
								c = '@';
							}
						}
						if (grid[y].test(i))
							c = '#';
						cout << c;
					}
					cout << '|' << endl;
				}

				if (bottom == 0)
					cout << "+-------+" << endl << endl;
				else
					cout << "   ...   " << endl << endl;
			};

			if (debug)
				Display();

			while (true)
			{
				// pushed by gas
				bool canMove = true;
				if (line[curDir] == '<')
				{
					if (debug)
						cout << "Push left" << endl;
					if (posX == 0)
					{
						canMove = false; // left edge
					}
					else
					{
						for (const auto &pos : curRock.pos)
						{
							if (grid[posY + pos.y].test(posX + pos.x - 1))
							{
								canMove = false;
								break;
							}
						}
					}
					if (canMove)
						posX--;
				}
				else if (line[curDir] == '>')
				{
					if (debug)
						cout << "Push right" << endl;
					if (posX + curRock.width == grid[0].size())
					{
						canMove = false; // right edge
					}
					else
					{
						for (const auto &pos : curRock.pos)
						{
							if (grid[posY + pos.y].test(posX + pos.x + 1))
							{
								canMove = false;
								break;
							}
						}
					}
					if (canMove)
						posX++;
				}
				curDir = (curDir + 1) % nbDirection;

				if (debug)
					Display();

				// gravity
				bool canFall = true;

				for (const auto &pos : curRock.pos)
				{
					size_t y = posY + pos.y - 1;
					if ((y < 0 || y > posY + pos.y) || grid[y].test(posX + pos.x))
					{
						canFall = false;
						break;
					}
				}

				if (canFall)
				{
					posY--;
					if (debug)
					{
						cout << "Gravity" << endl;
						Display();
					}
				}
				else
				{
					// push inside grid
					// and next rock
					for (const auto &pos : curRock.pos)
					{
						int	   x = posX + pos.x;
						size_t y = posY + pos.y;
						assert(grid[y].test(x) == false);
						grid[y].set(x, true);
					}

					maxHeight = max(maxHeight, posY + curRock.height);
					if (debug)
						Display();

					break;
				}
			}

			curShape = Shape((curShape + 1) % Shape::COUNT);
			heights.push_back(maxHeight);


			if (maxHeight >= 32)
			{
				State state;
				state.move	= curDir;
				state.shape = curShape;
				for (int x = 0; x < state.heightmap.size(); ++x)
				{
					// track whether there is a rock or not
					int height = 0;
					for (int bit = 0; bit < 32 && bit < maxHeight; ++bit)
					{
						if (grid[maxHeight - bit].test(x))
							height |= (1 << bit);
					}
					state.heightmap[x] = height;
				}

				cache[state]++;
				static size_t lastCycle		 = 0;
				static size_t sameCycleRun	 = 0;
				static size_t lastBiggestRun = 0;
				if (cache[state] >= 3)
				{
					const size_t nbCycle = cache[state];

					size_t cycleLength = 0;
					{
						for (const auto &[state, nb] : cache)
						{
							if (nb >= nbCycle - 1)
							{
								++cycleLength;
							}
						}
					}

					size_t minStep		  = step % cycleLength;
					size_t minHeight	  = heights[minStep - 1];
					size_t heightPerCycle = heights[step - 1] - heights[step - 1 - cycleLength];


					auto ComputeForStep = [&](size_t nbStep) {
						size_t nbCycleNeeded = floor(double(nbStep - minStep) / cycleLength);
						size_t remainderStep = (nbStep - minStep) % cycleLength;
						size_t totalHeight	 = heightPerCycle * nbCycleNeeded + heights[minStep - 1 + remainderStep];

						return totalHeight;
					};

					size_t heightPart1 = ComputeForStep(2022);
					assert(part1 == 0 || heightPart1 == part1);
					cout << "Part1:" << heightPart1 << endl;
					cout << "Part2:" << ComputeForStep(1000000000000llu) << endl;
					return;
				}
			}

			if (step == 2022)
			{
				part1 = maxHeight;
			}
		}

		cout << "Part1:" << part1 << endl;
		cout << "Part2 not found" << endl;
	}
} // namespace Day17