
//#define EXAMPLE


#ifdef EXAMPLE
constexpr int nbValve = 10;
bool		  debug	  = true;
#else
constexpr int nbValve = 52;
bool		  debug	  = false;
#endif

constexpr int maxStep = 26;

template<size_t Size, typename T>
struct SquareMatrix
{
	array<array<T, Size>, Size> mat { 0 };

	constexpr SquareMatrix() noexcept = default;
	constexpr SquareMatrix(const initializer_list<initializer_list<T>> &init) noexcept
	{
		int i = 0, j = 0;
		assert(init.size() == Size);
		for (const auto &row : init)
		{
			assert(row.size() == Size);
			for (const auto &v : row)
			{
				mat[i][j] = v;
				++j;
			}
			j = 0;
			++i;
		}
	}

	SquareMatrix &operator=(const SquareMatrix &rhs)
	{
		mat = rhs.mat;
		return *this;
	}

	SquareMatrix operator*(const SquareMatrix &rhs) const
	{
		SquareMatrix res;
		for (int i = 0; i < Size; ++i)
		{
			for (int j = 0; j < Size; ++j)
			{
				res.mat[i][j] = 0;
				for (int k = 0; k < Size; ++k)
				{
					res.mat[i][j] += mat[i][k] * rhs.mat[k][j];
				}
			}
		}
		return res;
	}

	SquareMatrix &operator*=(const SquareMatrix &rhs)
	{
		SquareMatrix res = *this * rhs;
		*this			 = res;
		return *this;
	}

	T &	   at(size_t i, size_t j) { return mat[i][j]; }
	size_t size() const { return Size; }
};

struct Edge;
struct Node
{
	int			   flow = 0;
	vector<Edge *> edges;
	string		   name;
	size_t		   index = -1;
};

struct Edge
{
	Node *from = nullptr;
	Node *to   = nullptr;
	int	  cost = 1;
};

vector<Node *> allNodes;
vector<Edge *> allEdges;

struct State
{
	Node *				 node				= nullptr;
	int					 remainingTimeForMe = 0;
	size_t				 totalFlow			= 0;
	std::bitset<nbValve> valves				= 0;

	bool operator==(State const &rhs) const
	{
		return valves == rhs.valves && node == rhs.node && remainingTimeForMe == rhs.remainingTimeForMe && totalFlow == rhs.totalFlow;
	}
};

template<>
struct std::hash<State>
{
	auto operator()(const State &s) const
	{
		size_t const h1 = reinterpret_cast<size_t>(s.node);
		size_t const h2 = s.totalFlow;
		size_t const h3 = s.valves.to_ullong();

		return ((h1 ^ (h2 << 1)) ^ (h3 << 3));
	}
};


void Solve()
{
	Node start;
	{
		unordered_map<string, Node *> nodes;
		nodes["AA"] = &start;

		ifstream is("Data16.txt");
		string	 line;
		while (getline(is, line))
		{
			char name[3];
			int	 flow = 0;
			sscanf_s(line.c_str(), "Valve %s has flow rate=%d", name, 3, &flow);

			string nodeName = name;
			Node * curNode	= nullptr;
			auto   it		= nodes.find(nodeName);
			if (it != nodes.end())
				curNode = it->second;
			else
			{
				curNode			= new Node;
				nodes[nodeName] = curNode;
			}
			curNode->index = allNodes.size();
			allNodes.push_back(curNode);
			curNode->flow = flow;
			curNode->name = nodeName;

			char *next_token;
			char *str = strtok_s(line.data() + line.find_last_of('v') + 3, ", ", &next_token);
			while (str)
			{
				string linkName = str;
				Node * node		= nullptr;
				auto   it		= nodes.find(linkName);
				if (it != nodes.end())
					node = it->second;
				else
				{
					node			= new Node;
					nodes[linkName] = node;
				}
				Edge *edge	 = nullptr;
				auto  itEdge = std::find_if(allEdges.begin(), allEdges.end(), [&](Edge *rhs) { return rhs->from == node && rhs->to == curNode; });
				if (itEdge != allEdges.end())
					edge = *itEdge;
				else
				{
					edge = new Edge { curNode, node, 1 };
					allEdges.push_back(edge);
				}
				curNode->edges.push_back(edge);
				str = strtok_s(NULL, ", ", &next_token);
			}
		}
	}

	// compute a distance map from adjacency matrix
	SquareMatrix<nbValve, size_t> adjacency;
	for (const auto &edge : allEdges)
	{
		assert(edge->from->index != edge->to->index);
		adjacency.at(edge->from->index, edge->to->index) = 1;
		adjacency.at(edge->to->index, edge->from->index) = 1;
	}
	SquareMatrix<nbValve, size_t> distance = adjacency;
	SquareMatrix<nbValve, size_t> temp	   = adjacency;

	for (size_t step = 2; step < min(maxStep, nbValve); ++step)
	{
		temp *= adjacency;
		for (int i = 0; i < adjacency.size(); ++i)
		{
			for (int j = 0; j < adjacency.size(); ++j)
			{
				if (i == j)
					continue;
				if (temp.at(i, j) > 0)
				{
					if (distance.at(i, j) == 0)
						distance.at(i, j) = step;
					else
						distance.at(i, j) = min(distance.at(i, j), step);
				}
			}
		}
	}

	// maximum graph distance
	size_t maxDist = 0;
	for (int i = 0; i < distance.size(); ++i)
	{
		for (int j = 0; j < distance.size(); ++j)
		{
			maxDist = max(maxDist, distance.at(i, j));
		}
	}

	// sort by flow rate
	array<size_t, nbValve> sortedValveIndices;
	for (int i = 0; i < nbValve; ++i)
		sortedValveIndices[i] = i;
	sort(sortedValveIndices.begin(), sortedValveIndices.end(), [](size_t lhs, size_t rhs) { return allNodes[lhs]->flow > allNodes[rhs]->flow; });

	// track all USEFUL valves
	decltype(State::valves) allValve;
	for (int i = 0; i < allNodes.size(); ++i)
		allValve.set(i, allNodes[i]->flow > 0);

	size_t				 maxFlow = 0;
	unordered_set<State> cache;


	auto MaxPressure = [&](const decltype(State::valves) &valves, size_t time, size_t nodeIdx) -> size_t {
		size_t sum = 0;
		for (size_t i = 0; i < valves.size() && time > 0 && time <= maxStep; ++i)
		{
			size_t index = sortedValveIndices[i];
			if (allValve.test(index) && !valves.test(index))
			{
				if (allNodes[index]->flow == 0)
					break;
				sum += allNodes[index]->flow * time;
				time -= distance.at(nodeIdx, index) + 1;
			}
		}

		return sum;
	};


	// all permutation
	vector<bitset<nbValve>> permutations;
	permutations.push_back(allValve);
	for (int bit = 0; bit < nbValve - 1; ++bit)
	{
		if (allValve.test(bit))
		{
			size_t size = permutations.size();
			for (size_t i = 0; i < size; ++i)
			{
				bitset<nbValve> bits = permutations[i];
				bits.flip(bit);
				permutations.push_back(bits);
			}
		}
	}


	size_t														count = 0;
	std::function<void(const State &, const bitset<nbValve> &)> Recurse;
	Recurse = [&](const State &state, const bitset<nbValve> &subset) {
		if (!cache.insert(state).second)
		{
			return; // already done
		}

		// if no more valve => return
		if (state.valves == subset)
		{
			++count;
			if (state.totalFlow > maxFlow)
			{
				maxFlow = state.totalFlow;
				if (debug)
					cout << "\tNew max by allValve:" << maxFlow << endl;
			}
			return;
		}
		// if no more time => return
		if (state.remainingTimeForMe <= 1)
		{
			++count;
			if (state.totalFlow > maxFlow)
			{
				maxFlow = state.totalFlow;
				if (debug)
					cout << "\tNew max by timeout:" << maxFlow << endl;
			}
			return;
		}

		// find the next valve to open
		vector<State> todo;
		bool		  bDone = false;
		for (size_t i = 0; i < state.valves.size(); ++i)
		{
			size_t index = sortedValveIndices[i];
			if (subset.test(index) &&	   // useful
				!state.valves.test(index)) // and not open
			{
				const size_t distForMe = distance.at(state.node->index, index);
				if (state.remainingTimeForMe > distForMe)
				{
					State newState = state;
					newState.node  = allNodes[index];
					newState.remainingTimeForMe -= distForMe + 1;
					newState.valves.set(index, true);
					newState.totalFlow += allNodes[index]->flow * newState.remainingTimeForMe;

					Recurse(newState, subset);
					bDone = true;
				}
			}
		}

		if (!bDone)
		{
			++count;
			if (state.totalFlow > maxFlow)
			{
				maxFlow = state.totalFlow;
				if (debug)
					cout << "\tNew max by other:" << maxFlow << endl;
			}
		}
	};

	State init;
	init.node				= &start;

	init.remainingTimeForMe = 30;
	Recurse(init, allValve);
	cout << "Part1:" << maxFlow << " over " << count << " states" << endl;

	count = 0;
	size_t realMax			= 0;
	init.remainingTimeForMe = 26;
	for (size_t per = 0; per < permutations.size(); per++)
	{
		const auto &permut = permutations[per];
		if (debug)
			cout << "Permutation " << per << ": " << permut.to_string() << endl;
		maxFlow = 0;
		cache.clear();
		Recurse(init, permut);
		size_t bak = maxFlow;
		maxFlow	   = 0;
		cache.clear();
		Recurse(init, allValve ^ permut);

		realMax = max(realMax, maxFlow + bak);
	}

	cout << "Part2:" << realMax << " over " << count << " states" << endl;
}