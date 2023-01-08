
enum Direction
{
	NONE = -1,
	NORTH,
	SOUTH,
	WEST,
	EAST
};

constexpr const char* DirectionToString(Direction dir)
{
	switch (dir)
	{
		case NORTH: return "NORTH";
		case SOUTH: return "SOUTH";
		case WEST: return "WEST";
		case EAST: return "EAST";
	}
}

struct Elf
{
	glm::ivec2 pos;
	Direction  wantedDir   = NONE;
	glm::ivec2 wantedPos;
	bitset<9>  elvesAround = 0;
};

glm::ivec2 minPos(0, 0);
glm::ivec2 maxPos(0, 0);

constexpr glm::ivec2 Grid[9]
{
	{-1, -1}, // top left
	{0, -1}, // top
	{1, -1}, // top right
	{-1, 0}, // left
	{0, 0}, // center
	{1, 0}, // right
	{-1, 1},// bottom left
	{0, 1}, // bottom 
	{1, 1}, // bottom right
};

void Solve()
{
	vector<Elf> elves;
	unordered_set<glm::ivec2> occupied;

	ifstream is("Data23.txt");
	string	 line;
	glm::ivec2 pos(0, 0);
	while (getline(is, line))
	{
		pos.x = 0;
		for (char c : line)
		{
			if (c == '#')
			{
				Elf elf;
				elf.pos = pos;
				elves.push_back(elf);
				minPos = glm::min(minPos, elf.pos);
				maxPos = glm::max(maxPos, elf.pos);
			}
			pos.x++;
		}

		pos.y++;
	}

	Direction					   currentDir = NORTH;
	unordered_map<glm::ivec2, int> mapChoices;
	mutex						   mapMutex;

	auto Display = [&]() {

		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
		cout << "Min:" << minPos << " max:" << maxPos << endl;
		cout << "Dir:" << DirectionToString(currentDir) << endl;
		int startY = csbiInfo.dwCursorPosition.Y + 2;

		for (int y = minPos.y; y <= maxPos.y; ++y)
		{
			printf("%2d ", y);
			for (int x = minPos.x-1; x <= maxPos.x+1; ++x)
			{
				cout << '.';
			}
			cout << endl;
		}
		for (const auto &elf : elves)
		{
			COORD pos = { elf.pos.x - minPos.x + 4 , elf.pos.y - minPos.y + startY };
			SetConsoleCursorPosition(hConsole, pos);
			WriteConsole(hConsole, "#", 1, NULL, NULL);
		}

		COORD pos = { maxPos.x - minPos.x, maxPos.y - minPos.y + startY };
		SetConsoleCursorPosition(hConsole, pos);
		WriteConsole(hConsole, "\n", 1, NULL, NULL);
	};


	bool debug = false;
	if (debug)
	{
		cout << "Init:" << endl;
		Display();
	}

	for (int round = 1; round <= 10000; ++round)
	{
		occupied.clear();
		mapChoices.clear();

		// first fetch all elves positions
		for (const auto &elf : elves)
		{
			occupied.insert(elf.pos);
		}

		// then count how many elves there are around each elf and decide where to go
		for_each(std::execution::par, elves.begin(), elves.end(), [&](Elf &elf) {
			elf.wantedDir = NONE;
			elf.wantedPos = elf.pos;
			for (int grid = 0; grid < _countof(Grid); ++grid)
			{
				elf.elvesAround.set(grid, occupied.find(elf.pos + Grid[grid]) != occupied.end());
			}

			if (elf.elvesAround.count() == 1) // idx 4 is the elf itself
				return;

			for (int dir = 0; dir < 4; ++dir)
			{
				Direction wantedDir		= Direction((currentDir + dir) % 4);
				int		  nbElvesAround = 0;
				switch (wantedDir)
				{
					case NORTH: nbElvesAround += elf.elvesAround[0] + elf.elvesAround[1] + elf.elvesAround[2]; break;
					case SOUTH: nbElvesAround += elf.elvesAround[6] + elf.elvesAround[7] + elf.elvesAround[8]; break;
					case WEST: nbElvesAround += elf.elvesAround[0] + elf.elvesAround[3] + elf.elvesAround[6]; break;
					case EAST: nbElvesAround += elf.elvesAround[2] + elf.elvesAround[5] + elf.elvesAround[8]; break;
				}

				if (nbElvesAround == 0)
				{
					elf.wantedDir = wantedDir;
					break;
				}
			}

			if (elf.wantedDir != NONE)
			{
				switch (elf.wantedDir)
				{
					case NORTH: elf.wantedPos += Grid[1]; break;
					case SOUTH: elf.wantedPos += Grid[7]; break;
					case WEST: elf.wantedPos += Grid[3]; break;
					case EAST: elf.wantedPos += Grid[5]; break;
				}

				std::lock_guard<std::mutex> guard(mapMutex);
				mapChoices[elf.wantedPos]++;
			}
		});

		// proceed to move
		int nbMove = 0;
		for (auto &elf : elves)
		{
			if (elf.wantedDir != NONE)
			{
				if (mapChoices[elf.wantedPos] == 1)
				{
					elf.pos = elf.wantedPos;
					minPos	= glm::min(minPos, elf.pos);
					maxPos	= glm::max(maxPos, elf.pos);
					++nbMove;
				}
			}
			else
			{
				// TODO remove from container to speed up
			}
		}

		if (nbMove == 0)
		{
			cout << "End after " << round << " rounds" << endl;

			if (debug)
				Display();
			break;
		}

		if (debug)
		{
			cout << endl << "After round " << round << endl;
			Display();
		}

		currentDir = Direction((currentDir + 1) % 4);
	}

	cout << "Part1:" << (maxPos.x - minPos.x+1) * (maxPos.y - minPos.y+1) - elves.size() << endl;
}