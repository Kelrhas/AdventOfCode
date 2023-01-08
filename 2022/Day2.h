


enum Sign
{
	ROCK	 = 1,
	PAPER	 = 2,
	SCISSORS = 3
};

enum Target
{
	LOSE	 = 'X',
	DRAW	 = 'Y',
	WIN		= 'Z'
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
	ifstream				 is("Data2.txt");

	//int	   score = 0;
	//string line;
	//while (getline(is, line))
	//{
	//	Sign enemySign = Sign(line[0] - 'A' + 1);
	//	Sign mySign	   = Sign(line[2] - 'A' + 1);
	//	
	//	score += GetScore(mySign, enemySign);
	//	score += (int)mySign;
	//}
	//
	//cout << "Part1:" << score << endl;

	int	   score = 0;
	string line;
	while (getline(is, line))
	{
		Sign enemySign = Sign(line[0] - 'A' + 1);
		Target target	   = Target(line[2]);
		Sign   mySign	 = GetSignToTarget(enemySign, target);

		score += GetScore(mySign, enemySign);
		score += (int)mySign;
	}

	cout << "Part2:" << score << endl;
}