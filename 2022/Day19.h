
enum RobotType
{
	ORE,
	CLAY,
	OBSIDIAN,
	GEODE,

	COUNT
};

struct Resource
{
	array<int, RobotType::COUNT> arr { 0 };
	//int &						 ore	  = arr[ORE];
	//int &						 clay	  = arr[CLAY];
	//int &						 obsidian = arr[OBSIDIAN];
	//int &						 geode	  = arr[3];

	bool EnoughFor(const Resource &cost) const { return arr[ORE] >= cost.arr[ORE] && arr[CLAY] >= cost.arr[CLAY] && arr[OBSIDIAN] >= cost.arr[OBSIDIAN]; }

	Resource &operator+=(Resource const &rhs)
	{
		for (int i = 0; i < arr.size(); ++i)
		{
			arr[i] += rhs.arr[i];
		}
		//ore += rhs.ore;
		//clay += rhs.clay;
		//obsidian += rhs.obsidian;
		//geode += rhs.geode;
		return *this;
	}
	Resource operator+(Resource const &rhs) const
	{
		Resource res = *this;
		res += rhs;
		return res;
	}
	Resource &operator-=(Resource const &rhs)
	{
		for (int i = 0; i < arr.size(); ++i)
		{
			arr[i] = std::max(0, arr[i] - rhs.arr[i]);
		}
		//ore		 = std::max(0, ore - rhs.ore);
		//clay	 = std::max(0, clay - rhs.clay);
		//obsidian = std::max(0, obsidian - rhs.obsidian);
		//geode	 = std::max(0, geode - rhs.geode);
		return *this;
	}
	Resource operator-(Resource const &rhs) const
	{
		Resource res = *this;
		res -= rhs;
		return res;
	}
	Resource operator*=(int nb)
	{
		for (int i = 0; i < arr.size(); ++i)
		{
			arr[i] *= nb;
		}
		//ore *= nb;
		//clay *= nb;
		//obsidian *= nb;
		//geode *= nb;
		return *this;
	}
	Resource operator*(int nb) const
	{
		Resource res = *this;
		res *= nb;
		return res;
	}
	int operator/(Resource const &rhs) const
	{
		// how many rhs we have in this
		if (arr[ORE] != 0 && rhs.arr[ORE] == 0)
			return 0;
		if (arr[CLAY] != 0 && rhs.arr[CLAY] == 0)
			return 0;
		if (arr[OBSIDIAN] != 0 && rhs.arr[OBSIDIAN] == 0)
			return 0;
		// ignore geode
		int res = 0;
		if (rhs.arr[ORE] > 0)
			res = glm::max(res, int(ceil(float(arr[ORE]) / rhs.arr[ORE])));
		if (rhs.arr[CLAY] > 0)
			res = glm::max(res, int(ceil(float(arr[CLAY]) / rhs.arr[CLAY])));
		if (rhs.arr[OBSIDIAN] > 0)
			res = glm::max(res, int(ceil(float(arr[OBSIDIAN]) / rhs.arr[OBSIDIAN])));
		return res;
	}
	bool operator==(Resource const &rhs) const
	{
		for (int i = 0; i < arr.size(); ++i)
		{
			if (arr[i] != rhs.arr[i])
				return false;
		}
		return true;
		//return ore == rhs.ore && clay == rhs.clay && obsidian == rhs.obsidian && geode == rhs.geode;
	}
};

template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, Resource const &res)
{
	out << "[" << res.arr[ORE] << "," << res.arr[CLAY] << "," << res.arr[OBSIDIAN] << "," << res.arr[GEODE] << "]";
	return out;
}

template<>
struct hash<Resource>
{
	size_t operator()(Resource const &r) const
	{
		size_t const h1 = hash<int> {}(r.arr[ORE]);
		size_t const h2 = hash<int> {}(r.arr[CLAY]);
		size_t const h3 = hash<int> {}(r.arr[OBSIDIAN]);
		size_t const h4 = hash<int> {}(r.arr[GEODE]);
		return ((h1 ^ (h2 << 1)) ^ (h3 << 3)) ^ (h4 << 5);
	}
};

vector<array<Resource, RobotType::COUNT>> blueprints;
vector<size_t>							  geodes;

struct State
{
	Resource resource;
	Resource robots;
	int		 time;

	bool operator==(State const &rhs) const { return resource == rhs.resource && robots == rhs.robots && time == rhs.time; }
};


template<>
struct hash<State>
{
	size_t operator()(State const &s) const
	{
		size_t const h1 = hash<Resource> {}(s.resource);
		size_t const h2 = hash<Resource> {}(s.robots);
		size_t const h3 = hash<int> {}(s.time);
		return (h1 ^ (h2 << 1)) ^ (h3 << 3);
	}
};

void Solve()
{
	ifstream is("Data19.txt");
	string	 line;

	while (getline(is, line))
	{
		array<Resource, RobotType::COUNT> cost;
		sscanf_s(line.c_str(),
				 "Blueprint %*d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot "
				 "costs %d ore and %d obsidian.",
				 &cost[ORE].arr[ORE],
				 &cost[CLAY].arr[ORE],
				 &cost[OBSIDIAN].arr[ORE],
				 &cost[OBSIDIAN].arr[CLAY],
				 &cost[GEODE].arr[ORE],
				 &cost[GEODE].arr[OBSIDIAN]);
		blueprints.push_back(cost);
	}
	geodes.resize(blueprints.size());


	queue<State>		 pending;
	unordered_set<State> cache;
	cache.reserve(500000);


	const int MAX_TIME = 32;
	const int NB_BLUEPRINT = MAX_TIME == 24 ? blueprints.size() : 3;

	for (int blue = 0; blue < NB_BLUEPRINT; ++blue)
	{
		cout << "Blueprint " << (blue + 1) << endl;

		State init;
		init.robots.arr[ORE] = 1;
		init.time		= 0;

		cache.insert(init);
		pending.push(init);

		Resource maxResNeeded;
		for (RobotType resource = ORE; resource < RobotType::COUNT; resource = RobotType(resource + 1))
		{
			for (RobotType robot = ORE; robot < RobotType::COUNT; robot = RobotType(robot + 1))
				maxResNeeded.arr[resource] = max(maxResNeeded.arr[resource], blueprints[blue][robot].arr[resource]);
		}

		int maxGeode = 0;
		while (!pending.empty())
		{
			const State s = pending.front();
			pending.pop();
			/* branch on decision
			 * - build only one robot
			 * - wait to have enough for a robot
			 */
			auto AddState = [&](const State &newState) {
				if (newState.time >= MAX_TIME)
				{
					if (maxGeode < s.resource.arr[GEODE])
					{
						cout << "New max: " << s.resource.arr[GEODE] << " from " << s.robots << " robots" << endl;
						maxGeode = s.resource.arr[GEODE];
					}
					return;
				}
				if (cache.insert(newState).second)
					pending.push(newState);
			};

			bool bCanDoSomething = false;

			// if we can build GEODE, focus on that
			if (s.resource.EnoughFor(blueprints[blue][GEODE]))
			{
				State newState = s;
				newState.resource += s.robots;
				newState.resource -= blueprints[blue][GEODE];
				newState.robots.arr[GEODE]++;
				newState.time++;
				AddState(newState);

				bCanDoSomething = true;
			}
			else
			{
				for (RobotType resource = ORE; resource < RobotType::COUNT; resource = RobotType(resource + 1))
				{
					// no use for other than geode at the final minute
					if (s.time >= MAX_TIME - 1 && resource != GEODE)
						continue;
					// or obsidian just before
					if (s.time >= MAX_TIME - 2 && resource != OBSIDIAN && resource != GEODE)
						continue;

					// we already have enough of this resource-collecting robot to build any robot
					if (resource != GEODE && s.robots.arr[resource] == maxResNeeded.arr[resource])
						continue;

					if (s.resource.EnoughFor(blueprints[blue][resource]))
					{
						State newState = s;
						newState.resource += s.robots;
						newState.resource -= blueprints[blue][resource];
						newState.robots.arr[resource]++;
						newState.time++;
						AddState(newState);

						bCanDoSomething = true;
					}
					else
					{
						Resource missing = blueprints[blue][resource] - s.resource;
						int		 nbTurn	 = missing / s.robots;

						if (nbTurn > 0 && s.time + nbTurn < MAX_TIME)
						{
							State newState = s;
							// wait for resources
							newState.resource += s.robots * nbTurn;
							newState.time += nbTurn;
							// then build robot
							newState.resource += s.robots;
							newState.resource -= blueprints[blue][resource];
							newState.robots.arr[resource]++;
							newState.time++;
							AddState(newState);

							bCanDoSomething = true;
						}
					}
				}
			}

			//if (!bCanDoSomething)
			{
				Resource finalRes = s.resource + s.robots * (MAX_TIME - s.time);
				if (maxGeode < finalRes.arr[GEODE])
				{
					cout << "New max: " << finalRes.arr[GEODE] << " from " << s.robots << " robots" << endl;
					maxGeode = finalRes.arr[GEODE];
				}
			}

		}

		cache.clear();


		cout << "\tmax geode: "
			 << ": " << maxGeode << endl
			 << endl;

		geodes[blue] = maxGeode;
	}

	if (MAX_TIME == 24)
	{
		size_t sum = 0;
		for (int i = 0; i < NB_BLUEPRINT; ++i)
		{
			sum += (i + 1) * geodes[i];
		}
		cout << "Part1:" << sum << endl;
	}
	else
	{
		size_t sum = 1;
		for (int i = 0; i < NB_BLUEPRINT; ++i)
		{
			sum *= geodes[i];
		}
		cout << "Part2:" << sum << endl;
	}
}