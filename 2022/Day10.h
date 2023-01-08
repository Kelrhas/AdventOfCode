
void Solve()
{
	ifstream			is("Data10.txt");
	std::vector<string> lines;
	{
		string line;
		while (getline(is, line))
		{
			lines.push_back(line);
		}
	}

	{
		int reg		  = 1;
		int cycle	  = 0;
		int lastCycle = 0;
		int lastReg	  = 0;
		int sum		  = 0;
		for (const auto &line : lines)
		{
			stringstream ss(line);
			string		 command;
			ss >> command;

			if (command.compare("noop") == 0)
			{
				++cycle;
			}
			else
			{
				cycle += 2;
				int operand;
				ss >> operand;
				reg += operand;
			}

			if (((cycle + 20) % 40) < ((lastCycle + 20) % 40))
			{
				int roundedCycle = (40 * ((cycle + 20) / 40) - 20);
				int count		 = roundedCycle * lastReg;
				cout << "Cycle " << roundedCycle << ":" << lastReg << endl;
				cout << "\t" << count << endl;
				sum += count;
			}

			lastCycle = cycle;
			lastReg	  = reg;
		}
		cout << "Part1:" << sum << endl;
	}

	{
		int reg		   = 1;
		int nextReg	   = 1;
		int cycle	   = 0;
		int endCycle   = 0;
		int commandIdx = 0;
		while (cycle < 240 && commandIdx < lines.size())
		{
			if (endCycle == cycle)
			{
				reg = nextReg;

				const auto & line = lines[commandIdx];
				stringstream ss(line);
				string		 command;
				ss >> command;

				if (command.compare("noop") == 0)
				{
					endCycle = cycle + 1;
				}
				else
				{
					int operand;
					ss >> operand;
					nextReg	 = reg + operand;
					endCycle = cycle + 2;
				}

				commandIdx++;
			}

			int column = cycle % 40;
			if (abs(reg - column) < 2)
				cout << '#';
			else
				cout << '.';


			cycle++;
			if (cycle % 40 == 0)
				cout << endl;
		}
	}
}