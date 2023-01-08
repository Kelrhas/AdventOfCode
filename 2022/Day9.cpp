#include "stdafx.h"

namespace Day9
{
	enum Direction
	{
		Left  = 'L',
		Right = 'R',
		Up	  = 'U',
		Down  = 'D',
	};

	template<size_t Nb>
	void MoveRope(array<glm::ivec2, Nb> &ropes)
	{
		for (int knot = 1; knot < ropes.size(); knot++)
		{
			int dist = ChebyshevDist(ropes[knot - 1], ropes[knot]);
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
	};

	void Solve()
	{
		cout << "--- Day 9: Rope Bridge ---" << endl;
		ifstream is("Data9.txt");

		vector<pair<Direction, int>> moves;

		string line;
		while (getline(is, line))
		{
			stringstream		 ss(line);
			pair<Direction, int> move;
			move.first = (Direction)ss.get();
			ss >> move.second;
			moves.push_back(move);
		}


		array<glm::ivec2, 2>	  ropesPart1 { glm::ivec2(0, 0) };
		array<glm::ivec2, 10>	  ropesPart2 { glm::ivec2(0, 0) };
		unordered_set<glm::ivec2> visitedPart1;
		unordered_set<glm::ivec2> visitedPart2;
		visitedPart1.insert(ropesPart1.back());
		visitedPart2.insert(ropesPart2.back());

		for (const auto &[dir, move] : moves)
		{
			// cout << "Move:" << (char)dir << " " << move << endl;
			for (int i = 0; i < move; ++i)
			{
				switch (dir)
				{
					case Left:
						ropesPart1.front().x -= 1;
						ropesPart2.front().x -= 1;
						break;
					case Right:
						ropesPart1.front().x += 1;
						ropesPart2.front().x += 1;
						break;
					case Up:
						ropesPart1.front().y -= 1;
						ropesPart2.front().y -= 1;
						break;
					case Down:
						ropesPart1.front().y += 1;
						ropesPart2.front().y += 1;
						break;
				}

				MoveRope(ropesPart1);
				MoveRope(ropesPart2);

				visitedPart1.insert(ropesPart1.back());
				visitedPart2.insert(ropesPart2.back());
			}
		}

		int sum = 0;
		cout << "Part1:" << visitedPart1.size() << endl;
		cout << "Part2:" << visitedPart2.size() << endl;
	}
} // namespace Day9