
//#define EXAMPLE

enum Direction
{
	NONE = -1,
	NORTH,
	SOUTH,
	WEST,
	EAST
};

struct Cell
{
	bitset<4> blizz = 0;
};

struct State
{
	glm::ivec2 pos	= glm::ivec2(0, 0);
	int		   step = 0;

	bool operator==(State const &rhs) const { return pos == rhs.pos; }
};

template<>
struct hash<State>
{
	size_t operator()(State const &s) const
	{
		const size_t h1 = hash<glm::ivec2> {}(s.pos);
		const size_t h2 = hash<int> {}(s.step);
		return h1 ^ (h2 << 3);
	}
};

void Solve()
{
#ifdef EXAMPLE
	constexpr glm::ivec2 SIZE(6, 4);
	bool				 debug = true;
#else
	constexpr glm::ivec2 SIZE(120, 25);
	bool				 debug = false;
#endif
	Cell	   grid[SIZE.y][SIZE.x];
	ifstream   is("Data24.txt");
	string	   line;
	glm::ivec2 pos(-1, -1);
	while (getline(is, line))
	{
		pos.x = -1;
		for (char c : line)
		{
			switch (c)
			{
				case '^': grid[pos.y][pos.x].blizz.set(NORTH, true); break;
				case 'v': grid[pos.y][pos.x].blizz.set(SOUTH, true); break;
				case '<': grid[pos.y][pos.x].blizz.set(WEST, true); break;
				case '>': grid[pos.y][pos.x].blizz.set(EAST, true); break;

				default: assert(false); [[fallthrough]];
				case '.':
				case '#': break;
			}

			++pos.x;
		}
		pos.y++;
	}

	auto Display = [&](const Cell _grid[SIZE.y][SIZE.x]) {
		for (int x = -1; x < SIZE.x + 1; ++x)
			cout << (x == 0 ? '.' : '#');
		cout << endl;
		for (int y = 0; y < SIZE.y; ++y)
		{
			cout << '#';
			for (int x = 0; x < SIZE.x; ++x)
			{
				const int nbBlizz = _grid[y][x].blizz.count();
				if (nbBlizz == 0)
					cout << '.';
				else if (nbBlizz >= 2)
					cout << nbBlizz;
				else if (_grid[y][x].blizz[NORTH])
					cout << '^';
				else if (_grid[y][x].blizz[SOUTH])
					cout << 'v';
				else if (_grid[y][x].blizz[WEST])
					cout << '<';
				else if (_grid[y][x].blizz[EAST])
					cout << '>';
			}
			cout << '#' << endl;
		}
		for (int x = -1; x < SIZE.x + 1; ++x)
			cout << (x == SIZE.x - 1 ? '.' : '#');
		cout << endl;
	};

	auto Advance = [&](const Cell _before[SIZE.y][SIZE.x], Cell _after[SIZE.y][SIZE.x]) {
		memset(_after, 0, sizeof(_before));
		for (int y = 0; y < SIZE.y; ++y)
		{
			for (int x = 0; x < SIZE.x; ++x)
			{
				_after[y][x].blizz.set(NORTH, _before[(y + 1) % SIZE.y][x].blizz[NORTH]);
				_after[y][x].blizz.set(SOUTH, _before[(y + SIZE.y - 1) % SIZE.y][x].blizz[SOUTH]);
				_after[y][x].blizz.set(WEST, _before[y][(x + 1) % SIZE.x].blizz[WEST]);
				_after[y][x].blizz.set(EAST, _before[y][(x + SIZE.x - 1) % SIZE.x].blizz[EAST]);
				/*if (_before[y][x].blizz[NORTH])
				{
					if (y == 0)
						_after[SIZE.y - 1][x].blizz.set(NORTH, true);
					else
						_after[y - 1][x].blizz.set(NORTH, true);
				}
				if (_before[y][x].blizz[SOUTH])
				{
					if (y == SIZE.y - 1)
						_after[0][x].blizz.set(SOUTH, true);
					else
						_after[y + 1][x].blizz.set(SOUTH, true);
				}
				if (_before[y][x].blizz[WEST])
				{
					if (x == 0)
						_after[y][SIZE.x - 1].blizz.set(WEST, true);
					else
						_after[y][x - 1].blizz.set(WEST, true);
				}
				if (_before[y][x].blizz[EAST])
				{
					if (x == SIZE.x - 1)
						_after[y][0].blizz.set(EAST, true);
					else
						_after[y][x + 1].blizz.set(EAST, true);
				}
				*/
			}
		}
	};

	if (debug)
	{
		cout << "Init" << endl;
		Display(grid);
	}

	const int minStep = std::lcm(SIZE.x, SIZE.y);
#if 0 // all positions are valid
	// try to prefetch all possible positions to prune computations
	unordered_set<glm::ivec2> freeSpots;
	{
		Cell gridCopy[SIZE.y][SIZE.x];
		memcpy(gridCopy, grid, sizeof(grid));
		for (int step = 0; step < minStep; ++step)
		{
			Cell temp[SIZE.y][SIZE.x];
			Advance(gridCopy, temp);
			swap(gridCopy, temp);
			if (debug)
			{
				cout << "Step " << (step + 1) << endl;
				Display(gridCopy);
				cout << endl;
			}
			for (int y = 0; y < SIZE.y; ++y)
			{
				for (int x = 0; x < SIZE.x; ++x)
				{
					if (gridCopy[y][x].blizz.none())
						freeSpots.insert({ x, y });
				}
			}
		}
	}

	cout << "Free spots:" << freeSpots.size() << "/" << SIZE.x * SIZE.y << endl;
#endif

	const glm::ivec2 start(0, 0);
	const glm::ivec2 end(SIZE.x - 1, SIZE.y - 1);


	vector<State>		 todo, todoCopy;
	unordered_set<State> cache;

	State init;
	init.pos  = glm::ivec2(0, -1);
	init.step = 0;
	todo.push_back(init);

	Cell gridCopy[SIZE.y][SIZE.x];
	bool reachedEnd = false;
	bool reachedStart = false;
	for (int step = 0;; ++step)
	{
		// advance step
		Advance(grid, gridCopy);
		swap(grid, gridCopy);
		if (debug)
		{
			cout << endl << "Step " << (step + 1) << ": " << todo.size() << " candidates" << endl;
			Display(grid);
		}

		if (todo.empty())
		{
			cout << "No more candidates at step " << (step + 1) << endl;
			break;
		}

		// check what every state can do, mutate and push if not visited
		for (State s : todo)
		{
			if (debug)
			{
				cout << "\ttesting " << s.pos << endl;
			}

			auto Push = [&](const State &_s) {
				auto it = cache.find(_s);
				if (it == cache.end())
				{
					cache.insert(_s);
					todoCopy.push_back(_s);
				}
				else
				{
					auto hint = it;
					hint++;
					cache.erase(it);
					cache.insert(hint, _s);
				}
			};
			s.step++;

			if (!reachedEnd && s.pos == end)
			{
				cout << "Reached end in " << s.step << " steps" << endl;
				todoCopy.clear();
				s.pos.y++;
				todoCopy.push_back(s);
				reachedEnd = true;
				break;
			}
			if (reachedEnd && !reachedStart && s.pos == start)
			{
				cout << "Reached start in " << s.step << " steps" << endl;
				todoCopy.clear();
				todoCopy.push_back(s);
				reachedStart = true;
				break;
			}
			if (reachedEnd && reachedStart && s.pos == end)
			{
				cout << "Reached end again in " << s.step << " steps" << endl;
				return;
			}

			if (s.pos.y == -1 || s.pos.y == SIZE.y ||
				(s.pos.y >= 0 && grid[s.pos.y][s.pos.x].blizz.none())) // wait
				Push(s);

			if (s.pos.y > 0) // NORTH
			{
				if (grid[s.pos.y - 1][s.pos.x].blizz.none())
				{
					State newState = s;
					newState.pos.y--;
					Push(newState);
				}
			}

			if (s.pos.y < SIZE.y - 1) // SOUTH
			{
				if (grid[s.pos.y + 1][s.pos.x].blizz.none())
				{
					State newState = s;
					newState.pos.y++;
					Push(newState);
				}
			}

			if (s.pos.x > 0) // WEST
			{
				if (grid[s.pos.y][s.pos.x - 1].blizz.none())
				{
					State newState = s;
					newState.pos.x--;
					Push(newState);
				}
			}

			if (s.pos.x < SIZE.x - 1) // EAST
			{
				if (grid[s.pos.y][s.pos.x + 1].blizz.none())
				{
					State newState = s;
					newState.pos.x++;
					Push(newState);
				}
			}
		}
		todo.clear();

		swap(todo, todoCopy);
		cache.clear();

	}
}