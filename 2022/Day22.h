#include <charconv>

//#define EXAMPLE
#ifdef EXAMPLE
bool debug = true;
#else
bool debug = false;
#endif

enum Tile
{
	EMPTY,
	GROUND,
	WALL
};

enum Direction
{
	RIGHT,
	DOWN,
	LEFT,
	UP
};

Direction TurnRight(Direction _dir)
{
	return Direction((_dir + 1) % 4);
}
Direction TurnLeft(Direction _dir)
{
	return Direction((_dir + 3) % 4);
}


constexpr glm::ivec2 VecFromDir(Direction dir)
{
	switch (dir)
	{
		case RIGHT: return glm::ivec2(1, 0);
		case DOWN: return glm::ivec2(0, 1);
		case LEFT: return glm::ivec2(-1, 0);
		case UP: return glm::ivec2(0, -1);
	}
	return glm::ivec2(0, 0);
}

struct Move
{
	size_t nb		= 0;
	bool   turnLeft = false;
};

using Quadrant = glm::ivec2;
using FaceInfo = vector<tuple<Direction, Quadrant, int>>; // toward what quadrant and in how many right turn

#ifdef EXAMPLE
constexpr int					  QUADRANT = 4;
constexpr size_t				  WIDTH	   = 4 * QUADRANT;
constexpr size_t				  HEIGHT   = 3 * QUADRANT;
unordered_map<Quadrant, FaceInfo> transfoHardcode {
	{ Quadrant { 2, 0 }, { { RIGHT, Quadrant { 3, 2 }, 2 }, { DOWN, Quadrant { 2, 1 }, 0 }, { LEFT, Quadrant { 1, 1 }, 3 }, { UP, Quadrant { 0, 1 }, 2 } } },
	{ Quadrant { 0, 1 }, { { RIGHT, Quadrant { 1, 1 }, 0 }, { DOWN, Quadrant { 2, 2 }, 2 }, { LEFT, Quadrant { 3, 2 }, 3 }, { UP, Quadrant { 2, 0 }, 2 } } },
	{ Quadrant { 1, 1 }, { { RIGHT, Quadrant { 2, 1 }, 0 }, { DOWN, Quadrant { 2, 2 }, 3 }, { LEFT, Quadrant { 0, 1 }, 0 }, { UP, Quadrant { 2, 0 }, 1 } } },
	{ Quadrant { 2, 1 }, { { RIGHT, Quadrant { 3, 2 }, 1 }, { DOWN, Quadrant { 2, 2 }, 0 }, { LEFT, Quadrant { 1, 1 }, 0 }, { UP, Quadrant { 2, 0 }, 0 } } },
	{ Quadrant { 2, 2 }, { { RIGHT, Quadrant { 3, 2 }, 0 }, { DOWN, Quadrant { 0, 1 }, 2 }, { LEFT, Quadrant { 1, 1 }, 1 }, { UP, Quadrant { 2, 1 }, 0 } } },
	{ Quadrant { 3, 2 }, { { RIGHT, Quadrant { 2, 0 }, 2 }, { DOWN, Quadrant { 0, 1 }, 1 }, { LEFT, Quadrant { 2, 2 }, 0 }, { UP, Quadrant { 2, 1 }, 3 } } },
};
#else
constexpr int					  QUADRANT = 50;
constexpr size_t				  WIDTH	   = 3 * QUADRANT;
constexpr size_t				  HEIGHT   = 4 * QUADRANT;
unordered_map<Quadrant, FaceInfo> faces {
	{ Quadrant { 1, 0 }, { { RIGHT, Quadrant { 2, 0 }, 0 }, { DOWN, Quadrant { 1, 1 }, 0 }, { LEFT, Quadrant { 0, 2 }, 2 }, { UP, Quadrant { 0, 3 }, 1 } } },
	{ Quadrant { 2, 0 }, { { RIGHT, Quadrant { 1, 2 }, 2 }, { DOWN, Quadrant { 1, 1 }, 1 }, { LEFT, Quadrant { 1, 0 }, 0 }, { UP, Quadrant { 0, 3 }, 0 } } },
	{ Quadrant { 1, 1 }, { { RIGHT, Quadrant { 2, 0 }, 3 }, { DOWN, Quadrant { 1, 2 }, 0 }, { LEFT, Quadrant { 0, 2 }, 3 }, { UP, Quadrant { 1, 0 }, 0 } } },
	{ Quadrant { 0, 2 }, { { RIGHT, Quadrant { 1, 2 }, 0 }, { DOWN, Quadrant { 0, 3 }, 0 }, { LEFT, Quadrant { 1, 0 }, 2 }, { UP, Quadrant { 1, 1 }, 1 } } },
	{ Quadrant { 1, 2 }, { { RIGHT, Quadrant { 2, 0 }, 2 }, { DOWN, Quadrant { 0, 3 }, 1 }, { LEFT, Quadrant { 0, 2 }, 0 }, { UP, Quadrant { 1, 1 }, 0 } } },
	{ Quadrant { 0, 3 }, { { RIGHT, Quadrant { 1, 2 }, 3 }, { DOWN, Quadrant { 2, 0 }, 0 }, { LEFT, Quadrant { 1, 0 }, 3 }, { UP, Quadrant { 0, 2 }, 0 } } },
};
#endif

void Solve()
{
	Tile grid[HEIGHT][WIDTH] { EMPTY };

	size_t		 y = 0;
	vector<Move> moves;
	ifstream	 is("Data22.txt");
	string		 line;
	while (getline(is, line))
	{
		if (line.empty())
		{
			getline(is, line);

			Move		move;
			string_view sv(line);
			while (!sv.empty())
			{
				size_t leftPos	= sv.find_first_of('L');
				size_t rightPos = sv.find_first_of('R');

				size_t turnPos;
				bool   turnLeft = false;
				if (leftPos != string::npos && rightPos != string::npos)
				{
					if (leftPos < rightPos)
					{
						turnPos	 = leftPos;
						turnLeft = true;
					}
					else
					{
						turnPos	 = rightPos;
						turnLeft = false;
					}
				}
				else if (leftPos != string::npos)
				{
					turnPos	 = leftPos;
					turnLeft = true;
				}
				else if (rightPos != string::npos)
				{
					turnPos	 = rightPos;
					turnLeft = false;
				}
				else
				{
					turnPos = sv.size();
				}

				string_view number = sv.substr(0, turnPos);
				from_chars(sv.data(), sv.data() + sv.size(), move.nb);
				sv.remove_prefix(min(turnPos + 1, sv.size()));
				moves.push_back(move);
				move.turnLeft = turnLeft;
			}
		}
		else
		{
			for (size_t x = 0; x < WIDTH && x < line.size(); ++x)
			{
				const char c = line[x];
				switch (c)
				{
					case ' ': grid[y][x] = EMPTY; break;
					case '.': grid[y][x] = GROUND; break;
					case '#': grid[y][x] = WALL; break;
					default: assert(false);
				}
			}
			++y;
		}
	}

	auto Display = [&]() {
		for (size_t y = 0; y < HEIGHT; ++y)
		{
			for (size_t x = 0; x < WIDTH; ++x)
			{
				Tile &tile = grid[y][x];
				cout << (tile == WALL ? '#' : tile == GROUND ? '.' : ' ');
			}
			cout << endl;
		}
	};

#ifdef EXAMPLE
	Display();
#endif

	Direction  currentDir = RIGHT;
	glm::ivec2 pos(0, 0);
	while (grid[0][pos.x] == EMPTY)
	{
		++pos.x;
	}

	cout << "Starting at " << pos << endl;

	auto GetNextPos = [&](glm::ivec2 pos, Direction dir) {
		glm::ivec2 vDir = VecFromDir(dir);

		const glm::ivec2 oldPos	 = pos;
		const Quadrant	 oldQuad = oldPos / QUADRANT;

		pos.x			 = int((pos.x + vDir.x + WIDTH) % WIDTH);
		pos.y			 = int((pos.y + vDir.y + HEIGHT) % HEIGHT);
		Quadrant newQuad = pos / QUADRANT;

		if (oldQuad != newQuad)
		{
			const FaceInfo info = faces.at(oldQuad);
			newQuad				= get<1>(info[currentDir]);
			int nbTurnRight		= get<2>(info[currentDir]);

			glm::ivec2 posInQuad(pos.x % QUADRANT, pos.y % QUADRANT);

			auto RotateLeft = [](const glm::ivec2 &v) -> glm::ivec2 {
				return glm::ivec2(v.y, QUADRANT - v.x - 1);
			};
			auto RotateRight = [](const glm::ivec2 &v) -> glm::ivec2 {
				return glm::ivec2(QUADRANT - v.y - 1, v.x);
			};

			switch (nbTurnRight)
			{
				case 0: break;
				case 1: posInQuad = RotateRight(posInQuad); break;
				case 2: posInQuad = RotateRight(RotateRight(posInQuad)); break;
				case 3: posInQuad = RotateLeft(posInQuad); break;
			}
			/*switch (nbTurnRight)
			{
				case 0:
					if (dir == LEFT || dir == RIGHT)
					{
						// y stays
						posInQuad.x = (posInQuad.x + QUADRANT - dir + 1) % QUADRANT;
					}
					else
					{
						// y stays
						posInQuad.y = (posInQuad.y + QUADRANT - dir + 2) % QUADRANT;
					}
					break;
				case 1:
				case 3:
					//if ((oldQuad.x + oldQuad.y)  == (newQuad.x + newQuad.y))
					if((nbTurnRight == 1 && (dir == UP ||  dir == DOWN)) ||
					   (nbTurnRight == 3 && (dir == LEFT ||  dir == RIGHT)))
					{
						// just inverse x & y
						swap(posInQuad.x, posInQuad.y);
					}
					else
					{
						// smart rotate stuff
						auto temp	= posInQuad.x;
						posInQuad.x = QUADRANT - posInQuad.y - 1;
						posInQuad.y = QUADRANT - temp - 1;
					}
					break;
				case 2:
					if (dir == LEFT || dir == RIGHT)
					{
						// x stays
						posInQuad.y = QUADRANT - posInQuad.y - 1;
					}
					else
					{
						// y stays
						posInQuad.x = QUADRANT - posInQuad.x - 1;
					}
					break;
				default: break;
			}*/

			pos = posInQuad + newQuad * QUADRANT;

			for (int i = 0; i < nbTurnRight; ++i)
				dir = TurnRight(dir);
		}

		Assert(grid[pos.y][pos.x] != EMPTY);


		std::pair<glm::ivec2, Direction> newPosAndDir(pos, dir);
		return newPosAndDir;
	};

	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		Move &move = *it;
		if (debug)
			cout << "Move:" << (move.turnLeft ? "left" : "right") << " and " << move.nb << endl;
		glm::ivec2 testPos = pos;
		if (it != moves.begin())
			currentDir = move.turnLeft ? TurnLeft(currentDir) : TurnRight(currentDir);
		for (size_t i = 0; i < move.nb; ++i)
		{
			auto [testPos, newDir] = GetNextPos(pos, currentDir);

			if (grid[testPos.y][testPos.x] == WALL)
				break;
			pos		   = testPos;
			currentDir = newDir;
			if (debug)
				cout << "\t" << pos << " facing " << currentDir << endl;
		}


		if (debug)
			cout << "{" << (pos.x+1) << "," << (pos.y+1) << "}," << endl;
	}

	cout << "Part2:" << pos << " facing " << currentDir << " -> " << (1000 * (pos.y + 1) + 4 * (pos.x + 1) + currentDir) << endl;
}