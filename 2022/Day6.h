

void Solve()
{
	std::ifstream is("Data6.txt");
	string		  line;

	int start = 0;
	is >> line;
	cout << line << endl;

	constexpr int back = 14;
	int idx = back;
	while (line[idx])
	{
		string_view view(line.c_str() + back- idx, back);
		bool		  br   = false;
		for (int i = 0; i < back; ++i)
		{
			for (int j = i+1; j < back; ++j)
			{
				if (line[idx - i] == line[idx - j])
				{
					br	  = true;
					idx += back - j;
					break;
				}
			}
			if (br)
				break;
		}

		if (!br)
		{
			start = idx;
			break;
		}
	}

	cout << "Part1:" << start + 1 << " / " << line.size() << endl;
}
