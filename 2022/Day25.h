
int SNAFUCharToDecDigit(char c)
{
	switch (c)
	{
		case '2': return 2;
		case '1': return 1;
		case '0': return 0;
		case '-': return -1;
		case '=': return -2;
	}

	assert(false);
	return 0;
}

char DecDigitToSNAFUChar(int i,  bool &borrow)
{
	switch (i)
	{
		case 4: borrow = true; return '-';
		case 3: borrow = true; return '=';
		case 2: borrow = false; return '2';
		case 1: borrow = false; return '1';
		case 0: borrow = false; return '0';
	}

	assert(false);
	return '0';
}

string DecToSNAFU(S64 i)
{
	string reverse;
	S64	   copy = i;
	for (int i = 0; copy != 0; ++i)
	{
		int	 remainder = copy % 5;
		bool borrow	   = false;
		reverse.push_back(DecDigitToSNAFUChar(remainder, borrow));
		if (borrow) // borrow
			copy += 5;
		copy /= 5;
	}
	std::reverse(reverse.begin(), reverse.end());
	return reverse;
}

S64 SNAFUToDec(const string &str)
{
	S64 nb = 0;
	for (char c : str)
	{
		nb *= 5;
		nb += SNAFUCharToDecDigit(c);
	}
	return nb;
}


void Solve()
{
	ifstream is("Data25.txt");
	string	 line;
	size_t	 sum = 0;
	while (getline(is, line))
	{
		S64 nb = SNAFUToDec(line);
		sum += nb;
		cout << line << " => " << nb << " => " << DecToSNAFU(nb) << endl;
	}

	cout << "Part1:" << sum << " => " << DecToSNAFU(sum) << endl;
}