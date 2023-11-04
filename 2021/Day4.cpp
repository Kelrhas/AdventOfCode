#include "stdafx.h"

namespace Day4
{
	void Solve()
	{
		cout << "--- Day 4: Giant Squid ---" << endl;
		std::ifstream is("Data4.txt");

		vector<int> vecNumbers;
		string		sLine;
		getline(is, sLine);

		int numberRound[100] = { 0 };

		for (const auto &range : sLine | views::split(','))
		{
			int num = stoi(string(range.begin(), range.end()));
			vecNumbers.push_back(num);
			numberRound[num] = vecNumbers.size();
		}

		// find at what round each number is called

		using Board = array<int, 25>;
		vector<Board> vecBoards;

		while (getline(is, sLine)) // first line should be empty
		{
			Board board;
			int index = 0;
			for (int i = 0; i < 5; ++i)
			{
				getline(is, sLine);
				for (const auto &range : sLine | views::split(' '))
				{
					if (range.size() >= 1)
					{
						string s(range.begin(), range.end());
						int	   val	   = s.empty() ? 0 : stoi(s);
						board[index++] = val;
					}
				}
			}

			vecBoards.emplace_back(board);
		}

		const size_t nbBoard = vecBoards.size();
		vector<int>	 roundWon(nbBoard, 0);
		int			 minRoundWinning   = vecNumbers.size();
		int			 firstBoardWinning = 0;
		int			 maxRoundWinning   = 0;
		int			 lastBoardWinning  = 0;
		for (int iBoard = 0; iBoard < nbBoard; ++iBoard)
		{
			const Board &board = vecBoards[iBoard];

			// find at which round the board wins by taking the highest number of rows/columns
			// and taking the minimum of those
			int winningRound = vecNumbers.size();
			for (int row = 0; row < 5; ++row)
			{
				int round	 = std::max({ numberRound[board[row * 5]],
										  numberRound[board[row * 5 + 1]],
										  numberRound[board[row * 5 + 2]],
										  numberRound[board[row * 5 + 3]],
										  numberRound[board[row * 5 + 4]] });
				winningRound = std::min(winningRound, round);

				round		 = std::max({ numberRound[board[row]],
										  numberRound[board[row + 5]],
										  numberRound[board[row + 10]],
										  numberRound[board[row + 15]],
										  numberRound[board[row + 20]] });
				winningRound = std::min(winningRound, round);
			}

			if (winningRound < minRoundWinning)
			{
				minRoundWinning = winningRound;
				firstBoardWinning = iBoard;
			}
			if (winningRound > maxRoundWinning)
			{
				maxRoundWinning	  = winningRound;
				lastBoardWinning = iBoard;
			}

			auto Display = [&]()
			{
				for (int y = 0; y < 5; ++y)
				{
					for (int x = 0; x < 5; ++x)
					{
						cout << setw(2) << board[x + y * 5] << " ";
					}
					cout << "\t\t";
					for (int x = 0; x < 5; ++x)
					{
						cout << setw(2) << numberRound[board[x + y * 5]] << " ";
					}
					cout << endl;
				}
				cout << "Winning at round " << winningRound << " with " << vecNumbers[winningRound-1] << endl;
			};

		}

		auto Score = [&](const Board &_board, int round) -> int{

			int sum = 0;
			for (int val : _board)
			{
				if (numberRound[val] > round)
					sum += val;
			}
			return sum * vecNumbers[round - 1];
		};

		cout << "Part1: " << Score(vecBoards[firstBoardWinning], minRoundWinning) << endl;
		cout << "Part2: " << Score(vecBoards[lastBoardWinning], maxRoundWinning) << endl;
	}
} // namespace Day4