

enum class State : U8
{
	NONE,
	EAST,
	SOUTH,
	MOVED
};

void Solve()
{

	std::ifstream is("Data25.txt");
	std::istream_iterator<string> start(is), end;
	std::vector<string> lines(start, end);

	size_t GRID_HEIGHT = lines.size();
	size_t GRID_WIDTH = 0;

	vector<State> grid;
	vector<State> gridTmp;

	for (int y = 0; y < GRID_HEIGHT; ++y)
	{
		if (GRID_WIDTH == 0)
			GRID_WIDTH = lines[y].size();
		else
			Assert(lines[y].size() == GRID_WIDTH);

		for (int x = 0; x < GRID_WIDTH; ++x)
		{
			char c = lines[y][x];
			grid.push_back(c == '.' ? State::NONE : c == '>' ? State::EAST : State::SOUTH);
		}
	}
	gridTmp = grid;

#ifdef EXAMPLE
	auto Display = [&](auto _grid) {
		for (int y = 0; y < GRID_HEIGHT; ++y)
		{
			for (int x = 0; x < GRID_WIDTH; ++x)
			{
				State eState = _grid[x + y * GRID_WIDTH];
				cout << (eState == State::NONE ? '.' : eState == State::EAST ? '>' : 'v');
			}
			cout << endl;
		}
	};

	Display(grid);
#endif

	bool bMoved = false;
	int step = 1;
	do
	{
		bMoved = false;
		// first EAST
		for (int y = 0; y < GRID_HEIGHT; ++y)
		{
			for (int x = 0; x < GRID_WIDTH; ++x)
			{
				int pos = x + y * GRID_WIDTH;
				State eState = grid[pos];
				if (eState == State::NONE)
				{
					int prevPos = ((x + GRID_WIDTH - 1) % GRID_WIDTH) + y * GRID_WIDTH;
					if (grid[prevPos] == State::EAST)
					{
						gridTmp[prevPos] = State::NONE;
						grid[prevPos] = State::MOVED;
						gridTmp[pos] = State::EAST;
						bMoved = true;
					}
					else
						gridTmp[pos] = eState;
				}
				else if (eState == State::MOVED)
					gridTmp[pos] = State::NONE;
				else
					gridTmp[pos] = eState;
			}
		}

		// then SOUTH
		for (int y = 0; y < GRID_HEIGHT; ++y)
		{
			for (int x = 0; x < GRID_WIDTH; ++x)
			{
				int pos = x + y * GRID_WIDTH;
				State eState = gridTmp[pos];
				if (eState == State::NONE)
				{
					int prevPos = x + ((y + GRID_HEIGHT - 1) % GRID_HEIGHT) * GRID_WIDTH;
					if (gridTmp[prevPos] == State::SOUTH)
					{
						grid[prevPos] = State::NONE;
						gridTmp[prevPos] = State::MOVED;
						grid[pos] = State::SOUTH;
						bMoved = true;
					}
					else
						grid[pos] = eState;
				}
				else if (eState == State::MOVED)
					gridTmp[pos] = State::NONE;
				else
					grid[pos] = eState;
			}
		}

		step++;
	} while (bMoved);


	cout << "Part1:" << (step - 1) << endl;
}