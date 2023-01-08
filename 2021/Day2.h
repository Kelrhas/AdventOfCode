

void Solve2()
{
	ifstream is("Data2.txt");

	glm::ivec2 pos{ 0,0 };

	string cmd;
	int val;
	while(is >> cmd >> val)
	{
		if (cmd == "forward")
			pos.x += val;
		else if (cmd == "down")
			pos.y += val;
		else if (cmd == "up")
			pos.y -= val;
	}

	cout << "Part1:" << pos.x * pos.y << endl;

	is.close();
	is.open("Data2.h");

	int aim = 0;
	pos = { 0,0 };
	while (is >> cmd >> val)
	{
		if (cmd == "forward")
		{
			pos.x += val;
			pos.y += aim * val;
		}
		else if (cmd == "down")
			aim += val;
		else if (cmd == "up")
			aim -= val;
	}

	cout << "Part2:" << pos.x * pos.y << endl;

}