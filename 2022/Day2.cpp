#include "stdafx.h"

namespace Day2
{
	enum Sign
	{
		ROCK	 = 1,
		PAPER	 = 2,
		SCISSORS = 3
	};

	enum Target
	{
		LOSE = 'X',
		DRAW = 'Y',
		WIN	 = 'Z'
	};


	int GetScore(Sign a, Sign b)
	{
		if (a == b)
			return 3;

		if (a == ROCK && b == SCISSORS || a == PAPER && b == ROCK || a == SCISSORS && b == PAPER)
			return 6;

		return 0;
	}

	Sign GetSignToTarget(Sign enemy, Target tar)
	{
		if (tar == DRAW)
			return enemy;

		if (tar == LOSE)
		{
			if (enemy == ROCK)
				return SCISSORS;
			if (enemy == PAPER)
				return ROCK;
			if (enemy == SCISSORS)
				return PAPER;
		}

		if (tar == WIN)
		{
			if (enemy == ROCK)
				return PAPER;
			if (enemy == PAPER)
				return SCISSORS;
			if (enemy == SCISSORS)
				return ROCK;
		}

		assert(false);
	}


	void Solve()
	{
		cout << "--- Day 2: Rock Paper Scissors ---" << endl;

		ifstream is("Data2.txt");

		int	   scorePart1 = 0;
		int	   scorePart2 = 0;
		string line;
		while (getline(is, line))
		{
			Sign enemySign = Sign(line[0] - 'A' + 1);
			Sign mySign	   = Sign(line[2] - 'X' + 1);

			scorePart1 += GetScore(mySign, enemySign);
			scorePart1 += (int)mySign;

			Target target = Target(line[2]);
			mySign		  = GetSignToTarget(enemySign, target);

			scorePart2 += GetScore(mySign, enemySign);
			scorePart2 += (int)mySign;
		}
		cout << "Part1:" << scorePart1 << endl;
		cout << "Part2:" << scorePart2 << endl;
	}
} // namespace Day2