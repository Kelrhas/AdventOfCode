

constexpr int NB_COLUMN = 9;


void Solve()
{
	std::ifstream		is("Data5.txt");
	std::vector<string> board;

	stack<char> crates[NB_COLUMN];

	string line;
	while (getline(is, line))
	{
		if (line.empty())
		{
			// init crates
			board.pop_back(); // last lign useless
	
			while (!board.empty())
			{
				for (int i = 0; i < NB_COLUMN; ++i)
				{
					char c = board.back()[i * 4 + 1];
					if(c != ' ')
						crates[i].push(c);
				}
				board.pop_back();
			}
	
			break;
		}
		else
		{
			board.push_back(line);
		}
	}
	
	//while (getline(is, line))
	//{
	//	stringstream ss(line);
	//	string		 word;
	//	int			 nb;
	//	int			 start, end;
	//	ss >> word >> nb >> word >> start >> word >> end;
	//
	//	for (int i = 0; i < nb; ++i)
	//	{
	//		char c = crates[start-1].top();
	//		crates[start-1].pop();
	//		crates[end-1].push(c);
	//	}
	//}
	//
	//string msg;
	//for (int i = 0; i < NB_COLUMN; ++i)
	//{
	//	msg.push_back(crates[i].top());
	//}
	//
	//
	//cout << "Part1:" << msg << endl;

	while (getline(is, line))
	{
		stringstream ss(line);
		string		 word;
		int			 nb;
		int			 start, end;
		ss >> word >> nb >> word >> start >> word >> end;

		stack<char> tmp;
		for (int i = 0; i < nb; ++i)
		{
			tmp.push(crates[start - 1].top());
			crates[start - 1].pop();
		}

		while (!tmp.empty())
		{
			crates[end - 1].push(tmp.top());
			tmp.pop();
		}
	}

	string msg;
	for (int i = 0; i < NB_COLUMN; ++i)
	{
		msg.push_back(crates[i].top());
	}


	cout << "Part2:" << msg << endl;
}
