#include "stdafx.h"

namespace Day5
{
	void Solve()
	{
		cout << "--- Day 5: Supply Stacks ---" << endl;
		std::ifstream is("Data5.txt");

		vector<stack<char>> crates;

		// init the board
		std::vector<string> board;
		string				line;

		size_t NB_COLUMN = 0;
		while (getline(is, line))
		{
			if (line.empty())
			{
				// init crates
				board.pop_back(); // last lign useless
				crates.resize(NB_COLUMN);

				while (!board.empty())
				{
					for (int i = 0; i < NB_COLUMN; ++i)
					{
						char c = board.back()[i * 4 + 1];
						if (c != ' ')
							crates[i].push(c);
					}
					board.pop_back();
				}

				break;
			}
			else
			{
				board.push_back(line);
				NB_COLUMN = max(NB_COLUMN, (line.size() + 1) / 4);
			}
		}


		struct Move
		{
			int nb;
			int from, to;
		};
		vector<Move> moves;
		while (getline(is, line))
		{
			stringstream ss(line);
			string		 word;
			Move		 move;
			ss >> word >> move.nb >> word >> move.from >> word >> move.to;
			moves.emplace_back(move);
		}


		vector<stack<char>> cratesPart2 = crates;

		for (const auto &move : moves)
		{
			for (int i = 0; i < move.nb; ++i)
			{
				char c = crates[move.from - 1].top();
				crates[move.from - 1].pop();
				crates[move.to - 1].push(c);
			}
		}

		cout << "Part1:";
		for (int i = 0; i < NB_COLUMN; ++i)
		{
			cout << crates[i].top();
		}
		cout << endl;

		for (const auto &move : moves)
		{
			stack<char> tmp;
			for (int i = 0; i < move.nb; ++i)
			{
				tmp.push(cratesPart2[move.from - 1].top());
				cratesPart2[move.from - 1].pop();
			}

			while (!tmp.empty())
			{
				cratesPart2[move.to - 1].push(tmp.top());
				tmp.pop();
			}
		}

		cout << "Part2:";
		for (int i = 0; i < NB_COLUMN; ++i)
		{
			cout << cratesPart2[i].top();
		}
		cout << endl;
	}
} // namespace Day5