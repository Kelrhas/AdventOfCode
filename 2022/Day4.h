

void Solve()
{
	std::ifstream is("Data4.txt");

	string line;
	int	   sum1 = 0;
	int	   sum2 = 0;
	int	   count = 0;
	while (getline(is, line))
	{
		stringstream ss(line);
		int			 first[2];
		int			 second[2];
		ss >> first[0];
		ss.get();
		ss >> first[1];
		ss.get();
		ss >> second[0];
		ss.get();
		ss >> second[1];


		if (first[0] >= second[0] && first[1] <= second[1]) // first inside second
			sum1++;
		else if (second[0] >= first[0] && second[1] <= first[1]) // second inside first
			sum1++;

		if (first[0] > second[1] || second[0] > first[1])
			sum2++;

		count++;
	}

	
	cout << "Part1:" << sum1 << " / " << count << endl;
	cout << "Part2:" << (count - sum2) << " / " << count << endl;
}
