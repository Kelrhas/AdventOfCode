

int GetPriority(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;

	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 27;

	assert(false);
}

void Solve()
{
	ifstream is("Data3.txt");
	istream_iterator<string> start(is), end;
	vector<string>			  lines(start, end);


	int sum = 0;
	for (const string &line : lines)
	{
		string_view comp1(line.c_str(), line.size() / 2);
		string_view comp2(line.c_str() + comp1.size(), comp1.size());
		assert(comp1.size() + comp2.size() == line.size());

		for (const char c : comp1)
		{
			size_t found = comp2.find_first_of(c);
			if (found != -1)
			{
				int score = GetPriority(c);
				//cout << "Found:" << c << " score:" << score << endl;
				sum += score;
				break;
			}
		}
	}

	cout << "Part1:" << sum << endl;

	
	sum = 0;
	for (int iLine = 0; iLine < lines.size(); iLine+=3)
	{
		int			   counts[52] = { 0 };

		for (char c = 'a'; c <= 'z' ; ++c)
		{
			if( lines[iLine].find_first_of(c) != -1 &&
				lines[iLine+1].find_first_of(c) != -1 &&
				lines[iLine+2].find_first_of(c) != -1)
			{
				sum += GetPriority(c);
				break;
			}
		}
		for (char c = 'A'; c <= 'Z' ; ++c)
		{
			if( lines[iLine].find_first_of(c) != -1 &&
				lines[iLine+1].find_first_of(c) != -1 &&
				lines[iLine+2].find_first_of(c) != -1)
			{
				sum += GetPriority(c);
				break;
			}
		}
	}

	cout << "Part2:" << sum << endl;
}