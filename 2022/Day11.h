

struct Monkey
{
	vector<size_t>					  items;
	std::function<size_t(size_t)> op;
	size_t						  testModulo;
	int							  targetTrue;
	int							  targetFalse;

	size_t countItem = 0;
};

//#define FIRST
//#define EXAMPLE
#ifdef EXAMPLE
vector<Monkey> monkeys =
{
	{
		{ 79, 98 },
		[](size_t old) { return old * 19; },
		23,
		2,
		3
	},
	{
		{ 54, 65, 75, 74 },
		[](size_t old) { return old + 6; },
		19,
		2,
		0
	},
	{
		{ 79, 60, 97 },
		[](size_t old) { return old * old; },
		13,
		1,
		3
	},
	{
		{ 74 },
		[](size_t old) { return old + 3; },
		17,
		0,
		1
	},
};
#else
vector<Monkey> monkeys =
{
	{
		{ 91, 54, 70, 61, 64, 64, 60, 85 },
		[](size_t old) { return old * 13; },
		2,
		5,
		2
	},
	{
		{ 82 },
		[](size_t old) { return old + 7; },
		13,
		4,
		3
	},
	{
		{ 84, 93, 70 },
		[](size_t old) { return old + 2; },
		5,
		5,
		1
	},
	{
		{ 78, 56, 85, 93 },
		[](size_t old) { return old *2; },
		3,
		6,
		7
	},
	{
		{ 64, 57, 81, 95, 52, 71, 58 },
		[](size_t old) { return old * old; },
		11,
		7,
		3
	},
	{
		{ 58, 71, 96, 58, 68, 90 },
		[](size_t old) { return old + 6; },
		17,
		4,
		1
	},
	{
		{ 56, 99, 89, 97, 81 },
		[](size_t old) { return old + 1; },
		7,
		0,
		2
	},
	{
		{ 68, 72 },
		[](size_t old) { return old + 8; },
		19,
		6,
		0
	},
};
#endif // EXAMPLE

void Solve()
{
	int idx = 0;

	size_t maxModulo = 1;
	for (auto &monkey : monkeys)
	{
		maxModulo *= monkey.testModulo;
	}

	bool debug = false;
#ifdef FIRST
	for (int round = 0; round < 20; ++round)
#else
	for (int round = 0; debug ? (round < 20) : (round < 10000); ++round)
#endif
	{
		int	 m	   = 0;
		for (auto &monkey : monkeys)
		{
			if (debug)
			{
				cout << "Monkey " << m++ << endl;
			}
			for (auto &item : monkey.items)
			{
				if (debug)
					cout << "\tBefore " << item;

				if (debug && item == 35834)
					__debugbreak();

				item %= maxModulo;
				item = monkey.op(item);
				item %= maxModulo;

				if (debug)
					cout << " after " << item << " divisible by " << monkey.testModulo << endl;
#ifdef FIRST
				item /= 3;
#endif

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

		#ifdef FIRST
		{
			cout << "After round " << (round + 1) << endl;
			for (int i = 0; i < monkeys.size(); ++i)
			{
				cout << "Monkey " << i << ":";
				for (auto &item : monkeys[i].items)
				{
					cout << ' ' << item << ',';
				}
				cout << endl;
			}

			for (int i = 0; i < monkeys.size(); ++i)
			{
				cout << "Monkey " << i << " inspected " << monkeys[i].countItem << " items" << endl;
			}
			cout << endl;
		}
		#else
		if (debug || round == 0 || round == 19 || (round % 1000) == 999)
		{
			cout << "After round " << (round + 1) << endl;
			for (int i = 0; i < monkeys.size(); ++i)
			{
				cout << "Monkey " << i << " inspected " << monkeys[i].countItem << " items" << endl;
			}
			cout << endl;
		}
		#endif
	}

	cout << endl;

	std::sort(monkeys.begin(), monkeys.end(), [](const auto &lhs, const auto &rhs) { return lhs.countItem > rhs.countItem; });
	
	cout << "Result:" << monkeys[0].countItem * monkeys[1].countItem << endl;
}
