
enum Direction
{
	Left = 'L',
	Right = 'R',
	Up = 'U',
	Down = 'D',
};

void Solve()
{
	ifstream is("Data9.txt");

	vector<pair<Direction, int>> moves;

	string line;
	while (getline(is, line))
	{
		stringstream ss(line);
		pair<Direction, int> move;
		move.first = (Direction)ss.get();
		ss >> move.second;
		moves.push_back(move);
	}


	array<glm::vec2, 2>	 ropes { glm::vec2(0, 0) }; // for part 1
	array<glm::vec2, 10>	 ropes { glm::vec2(0, 0) };
	unordered_set<glm::vec2> visited;
	visited.insert(ropes.back());

	for (const auto &[dir, move] : moves)
	{
		//cout << "Move:" << (char)dir << " " << move << endl;
		for (int i = 0; i < move; ++i)
		{
			switch (dir)
			{
				case Left: ropes.front().x -= 1; break;
				case Right: ropes.front().x += 1; break;
				case Up: ropes.front().y -= 1; break;
				case Down: ropes.front().y += 1; break;
			}

			for (int knot = 1; knot < ropes.size(); knot++)
			{
				int dist = ChebyshevDist(ropes[knot-1], ropes[knot]);
				if (dist >= 2)
				{
					if (ropes[knot - 1].x == ropes[knot].x)
						ropes[knot].y += glm::sign(ropes[knot - 1].y - ropes[knot].y);
					else if (ropes[knot - 1].y == ropes[knot].y)
						ropes[knot].x += glm::sign(ropes[knot - 1].x - ropes[knot].x);
					else // diagonal
					{
						ropes[knot].x += glm::sign(ropes[knot - 1].x - ropes[knot].x);
						ropes[knot].y += glm::sign(ropes[knot - 1].y - ropes[knot].y);
					}
				}
			}

			visited.insert(ropes.back());
			//cout << "\t" << tail << endl;
		}
	}

	int sum = 0;
	cout << "Part2:" << visited.size() << endl;
}