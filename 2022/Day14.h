
enum State
{
	AIR,
	ROCK,
	SAND
};

void Solve()
{
	int minX = 10000, maxX = 0, minY = 10000, maxY = 0;
	{
		ifstream is("Data14.txt");
		string	 line;
		while (getline(is, line))
		{
			stringstream ss(line);
			while (!ss.eof())
			{
				int	   x;
				int	   y;
				string word;
				ss >> x;
				ss.get();
				ss >> y;
				ss >> word;
				minX = min(minX, x);
				maxX = max(maxX, x);
				minY = min(minY, y);
				maxY = max(maxY, y);
			}
		}
	}

	const int startX = 500;
	const int startY = 0;
	int overflow = maxY - minY+3;

	minX = min(minX, startX) - overflow;
	maxX = max(maxX, startX) + overflow;
	minY = min(minY, startY);
	maxY = max(maxY+2, startY);

	int width  = maxX - minX + 1;
	int height = maxY - minY + 1;

	State **grid = new State *[height];
	for (int h = 0; h < height; ++h)
		grid[h] = new State[width] { AIR };

	for (int w = 0; w < width; ++w)
		grid[height - 1][w] = ROCK;

	ifstream is("Data14.txt");
	string	 line;
	string	 word;
	while (getline(is, line))
	{
		stringstream ss(line);
		int			 lastX=-1, lastY=-1;
		do
		{
			int	   x;
			int	   y;
			ss >> x;
			ss.get();
			ss >> y;
			ss >> word;

			grid[y - minY][x - minX] = ROCK;

			if (x == lastX)
			{
				int delta = y - lastY;
				for (int h = lastY; h != y; h += glm::sign(delta))
				{
					grid[h - minY][x - minX] = ROCK;
				}
			}
			else if (y == lastY)
			{
				int delta = x - lastX;
				for (int w = lastX; w != x; w += glm::sign(delta))
				{
					grid[y - minY][w - minX] = ROCK;
				}
			}

			lastX = x;
			lastY = y;

		} while (!ss.eof());
	}

	bool debug = false;
	int sum = 0;
	while (true)
	{
		int sandX = startX;
		int sandY = startY;
		if (debug)
		{
			for (int h = 0; h < height; ++h)
			{
				for (int w = 0; w < width; ++w)
				{
					if (w + minX == startX && h + minY == startY)
						cout << '+';
					else
					{
						switch (grid[h][w])
						{
							case ROCK: cout << "#"; break;
							case SAND: cout << "o"; break;
							default: cout << ".";
						}
					}
				}
				cout << endl;
			}
		}

		// step

		bool finish = false;
		++sum;
		while (true)
		{
			while (grid[sandY - minY + 1][sandX - minX] == AIR)
			{
				sandY++;
				if (sandY + 1 == height)
				{
					//finish = true;
					break;
				}
			}

			if (finish)
				break;

			if (grid[sandY - minY + 1][sandX - minX - 1] == AIR)
			{
				sandX--;
				sandY++;
			}
			else if (grid[sandY - minY + 1][sandX - minX + 1] == AIR)
			{
				sandX++;
				sandY++;
			}
			else
			{
				grid[sandY - minY][sandX - minX] = SAND;
				if (sandX == startX && sandY == startY)
					finish = true;
				break;
			}

			if (sandY + 1 == height)
			{
				//finish = true;
				break;
			}
		}

		if (finish)
			break;

		if (debug)
		{
			Sleep(100);
			system("cls");
		}
	}


	//cout << "Part1:" << sum-1 << endl;
	cout << "Part2:" << sum << endl;
}
