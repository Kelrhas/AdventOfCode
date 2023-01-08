#include "stdafx.h"

namespace Day11
{
	struct Monkey
	{
		vector<size_t>						  items;
		std::function<size_t(size_t, size_t)> op;
		size_t								  operand;
		size_t								  testModulo;
		int									  targetTrue;
		int									  targetFalse;


		size_t countItem = 0;
	};


	void Solve()
	{
		cout << "--- Day 11: Monkey in the Middle ---" << endl;

		vector<Monkey> monkeys;

		ifstream is("Data11.txt");
		string	 line, word;
		while (getline(is, line))
		{
			if (line.empty())
				continue;

			Monkey monk;

			// items
			getline(is, line);
			stringstream ss(line);
			ss >> word >> word;
			while (ss)
			{
				ss.ignore(1); //,
				if (!ss)
					break;
				int a;
				ss >> a;
				monk.items.push_back(a);
			}

			// operation
			getline(is, line);
			size_t								  pos = line.find("old") + 4;
			std::function<size_t(size_t, size_t)> func;
			if (line[pos] == '+')
			{
				monk.op = std::plus();
			}
			else if (line[pos] == '*')
			{
				monk.op = std::multiplies();
			}
			else
			{
				cerr << "Operator not recognized:" << line[pos] << endl;
			}
			pos += 2;
			if (line[pos] == 'o' && line.size() > pos + 2 && line[pos + 1] == 'l' && line[pos + 2] == 'd')
			{
				// "old" case
				monk.op		 = powers<size_t>();
				monk.operand = 2;
			}
			else
			{
				monk.operand = atoi(line.c_str() + pos);
			}


			// test
			getline(is, line);
			int mod;
			sscanf_s(line.c_str() + line.find_first_of(':'), ": divisible by %d", &mod);
			monk.testModulo = mod;
			getline(is, line);
			sscanf_s(line.c_str() + line.find_first_of(':'), ": throw to monkey %d", &monk.targetTrue);
			getline(is, line);
			sscanf_s(line.c_str() + line.find_first_of(':'), ": throw to monkey %d", &monk.targetFalse);

			monkeys.push_back(monk);
		}

		size_t maxModulo = 1;
		for (auto &monkey : monkeys)
		{
			maxModulo *= monkey.testModulo;
		}

		bool debugAsExample = false;
		bool debug			= false;

		auto Round = [&](vector<Monkey> &monkeys, int round, bool reduce) {
			for (auto &monkey : monkeys)
			{
				for (auto &item : monkey.items)
				{
					if (debug)
						cout << "\tBefore " << item;

					if (debug && item == 35834)
						__debugbreak();

					item %= maxModulo;
					item = monkey.op(item, monkey.operand);
					item %= maxModulo;

					if (debug)
						cout << " after " << item << " divisible by " << monkey.testModulo << endl;

					if (reduce)
						item /= 3;

					auto Test = [](int item, size_t modulo) -> bool {
						return (item % modulo) == 0;
					};

					if (Test(item, monkey.testModulo))
					{
						monkeys[monkey.targetTrue].items.push_back(item);
					}
					else
					{
						monkeys[monkey.targetFalse].items.push_back(item);
					}
				}
				monkey.countItem += monkey.items.size();
				monkey.items.clear();
			}
		};


		auto monkeysCopy = monkeys;
		for (int round = 1; round <= 20; ++round)
		{
			Round(monkeys, round, true);
		}
		std::sort(monkeys.begin(), monkeys.end(), [](const auto &lhs, const auto &rhs) { return lhs.countItem > rhs.countItem; });

		cout << "Part1:" << monkeys[0].countItem * monkeys[1].countItem << endl;


		monkeys = monkeysCopy;
		for (int round = 1; round <= 10000; ++round)
		{
			Round(monkeys, round, false);
		}
		std::sort(monkeys.begin(), monkeys.end(), [](const auto &lhs, const auto &rhs) { return lhs.countItem > rhs.countItem; });

		cout << "Part2:" << monkeys[0].countItem * monkeys[1].countItem << endl;
	}
} // namespace Day11