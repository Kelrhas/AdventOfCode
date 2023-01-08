#include "stdafx.h"

namespace Day24
{
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
} // namespace Day24

template<>
struct hash<Day24::State>
{
	size_t operator()(Day24::State const &s) const
	{
		const size_t h1 = hash<glm::ivec2> {}(s.pos);
		const size_t h2 = hash<int> {}(s.step);
		return h1 ^ (h2 << 3);
	}
};

namespace Day24
{
	void Solve()
	{
		cout << "--- Day 24: Blizzard Basin ---" << endl;
		bool debug = false;

		glm::ivec2			 SIZE(0,0);
		vector<vector<Cell>> grid;
		ifstream			 is("Data24.txt");
		string				 line;
		glm::ivec2			 pos(-1, -1);

		while (getline(is, line))
		{
			if (line[2] != '#')
			{
				grid.emplace_back();

				pos.x = -1;
				for (char c : line)
				{
					if (c == '#')
					{
						++pos.x;
						continue;
					}

					grid.back().emplace_back();
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
			}
			pos.y++;
		}
		SIZE.y = grid.size();
		SIZE.x = grid.front().size();

		auto Display = [&](const vector<vector<Cell>> &_grid) {
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

		auto Advance = [&](const vector<vector<Cell>> &_before, vector<vector<Cell>> &_after) {
			_after = _before;
			for (int y = 0; y < SIZE.y; ++y)
			{
				for (int x = 0; x < SIZE.x; ++x)
				{
					_after[y][x].blizz.set(NORTH, _before[(y + 1) % SIZE.y][x].blizz[NORTH]);
					_after[y][x].blizz.set(SOUTH, _before[(y + SIZE.y - 1) % SIZE.y][x].blizz[SOUTH]);
					_after[y][x].blizz.set(WEST, _before[y][(x + 1) % SIZE.x].blizz[WEST]);
					_after[y][x].blizz.set(EAST, _before[y][(x + SIZE.x - 1) % SIZE.x].blizz[EAST]);
				}
			}
		};

		if (debug)
		{
			cout << "Init" << endl;
			Display(grid);
		}

		const glm::ivec2 start(0, 0);
		const glm::ivec2 end(SIZE.x - 1, SIZE.y - 1);


		vector<State>		 todo, todoCopy;
		unordered_set<State> cache;

		State init;
		init.pos  = glm::ivec2(0, -1);
		init.step = 0;
		todo.push_back(init);

		vector<vector<Cell>> gridCopy;
		bool reachedEnd	  = false;
		bool reachedStart = false;
		for (int step = 0;; ++step)
		{
			// advance step
			Advance(grid, gridCopy);
			std::swap(grid, gridCopy);
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
					cout << "Part1:" << s.step << endl;
					todoCopy.clear();
					s.pos.y++;
					todoCopy.push_back(s);
					reachedEnd = true;
					break;
				}
				if (reachedEnd && !reachedStart && s.pos == start)
				{
					if (debug)
						cout << "Reached start in " << s.step << " steps" << endl;
					todoCopy.clear();
					todoCopy.push_back(s);
					reachedStart = true;
					break;
				}
				if (reachedEnd && reachedStart && s.pos == end)
				{
					cout << "Part2:" << s.step << endl;
					return;
				}

				if (s.pos.y == -1 || s.pos.y == SIZE.y || (s.pos.y >= 0 && grid[s.pos.y][s.pos.x].blizz.none())) // wait
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

				if (s.pos.y >= 0 && s.pos.y < SIZE.y)
				{
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
			}
			todo.clear();

			std::swap(todo, todoCopy);
			cache.clear();
		}
	}
} // namespace Day24