
int ROOM_DEPTH = 2;

static const int aMoveCost[4]{ 1, 10, 100, 1000 };

struct Amphipod
{
	using Pos = glm::vec<2, int8_t>;

	enum Type { A, B, C, D } eType;
	Pos pos{ 0, 0 };
	bool bGoodPlace = false;

	U8 TargetPosX() const { return (eType + 1) * 2; }
	bool InGoodRoom() const { return pos.x == TargetPosX() && pos.y > 0; }
	U64 MoveCost() const { return aMoveCost[(int)eType]; }

	U64 MaxEnergyRequired() const
	{
		Pos targetPos{ TargetPosX(), ROOM_DEPTH };
		if (!InGoodRoom())
			targetPos.y = -targetPos.y;
		return MoveCost() * ManhatanDist(pos, targetPos);
	}

	bool operator==(const Amphipod& rhs) const
	{
		return eType == rhs.eType && pos == rhs.pos;
	}

	bool operator<(const Amphipod& rhs) const
	{
		if (eType != rhs.eType) return eType < rhs.eType;
		if (pos.x != rhs.pos.x) return pos.x < rhs.pos.x;
		return pos.y < rhs.pos.y;
	}
};

template<>
struct std::hash<Amphipod> {
	auto operator() (const Amphipod& a) const {
		size_t const h1(std::hash<U8>{}((U8)a.eType));
		size_t const h2(std::hash<U8>{}(a.pos.x));
		size_t const h3(std::hash<U8>{}(a.pos.y));
		return (h1 ^ (h2 << 1)) ^ (h3 << 3);
	}
};

using State = vector<Amphipod>;
unordered_map<State, U64, vector_unordered_hash<Amphipod>> cacheDist;
unordered_map<State, State, vector_unordered_hash<Amphipod>> cachePrev;

U64 EstimateRemainingEnergy(const State& state)
{
	U64 energy = 0;
	for (const auto& pod : state)
		energy += pod.MaxEnergyRequired();
	return energy;
}

template<> struct std::less<State>
{
	bool operator()(const State& lhs, const State& rhs) const
	{
		return (cacheDist[lhs] + EstimateRemainingEnergy(lhs)) > (cacheDist[rhs] + EstimateRemainingEnergy(rhs));
	}
};
priority_queue<State> queueStates;

bool IsFinished(const State& state)
{
	for (auto& a : state)
	{
		if (!a.InGoodRoom())
			return false;
	}
	return true;
};

U64 best = 0xFFFFFFFFFFFFFFFF;
bool AddCandidate(const State& newState, U64 uNewEnergy, const State& previousState)
{
	for (const auto& pod : newState)
	{
		Assert(pod.pos.x >= 0 && pod.pos.x <= 10);
		Assert(pod.pos.y >= 0 && pod.pos.y <= ROOM_DEPTH);
	}

	if (auto it = cacheDist.find(newState); it != cacheDist.end())
	{
		Assert(!IsFinished(newState));

		if (uNewEnergy < it->second)
		{
			it->second = uNewEnergy;
			cachePrev[newState] = previousState;
			//queueStates.push( newState ); // ?
		}
	}
	else
	{
		cacheDist[newState] = uNewEnergy;
		cachePrev[newState] = previousState;

		if (IsFinished(newState))
		{
			if (uNewEnergy < best)
				cout << "Finished with " << uNewEnergy << endl;
			best = min(best, uNewEnergy);
			return true;
		}
		else
		{
			queueStates.push(newState);
		}
	}
	return false;
};

bool CheckRoomAvailable(const State& s, const Amphipod& pod)
{
	U8 nbInRoom = 0;
	for (auto& a : s)
	{
		if (a.pos.x == pod.TargetPosX() && a.pos.y > 0)
		{
			if (a.eType != pod.eType) // other type in room, forbidden
				return false;
			nbInRoom++;
		}

		if (nbInRoom == ROOM_DEPTH) // room full
			return false;
	}

	return true;
};


void Display(const State& _state)
{
	cout << "#############" << endl;
	cout << "#";
	for (int i = 0; i <= 10; ++i)
	{
		if (auto it = find_if(_state.begin(), _state.end(), [i](const Amphipod& pod) { return pod.pos.x == i && pod.pos.y == 0; }); it != _state.end())
		{
			SetConsoleTextAttribute(hConsole, it->bGoodPlace ? CONSOLE_GREEN : CONSOLE_WHITE);
			cout << (char)(it->eType + 'A');
			SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
		}
		else
			cout << '.';
	}
	cout << "#" << endl;
	for (int y = 1; y <= ROOM_DEPTH; ++y)
	{
		cout << (y == 1 ? "###" : "  #");
		for (int i = 2; i <= 8; i += 2)
		{
			if (auto it = find_if(_state.begin(), _state.end(), [i, y](const Amphipod& pod) { return pod.pos.x == i && pod.pos.y == y; }); it != _state.end())
			{
				SetConsoleTextAttribute(hConsole, it->bGoodPlace ? CONSOLE_GREEN : CONSOLE_WHITE);
				cout << (char)(it->eType + 'A');
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
			}
			else
				cout << '.';
			cout << '#';
		}
		cout << (y == 1 ? "##" : "") << endl;
	}
	cout << "  #########  " << endl;
};

void Solve()
{
	std::ifstream is("Data23.txt");

	State initState;

	Amphipod::Pos pos{ -1, -1 };
	string line;
	while (getline(is, line))
	{
		pos.x = -1;
		for (char c : line)
		{
			if(c != '#' && c != '.' && c != ' ')
				initState.push_back({ (Amphipod::Type)(c - 'A'), pos });
			pos.x++;
		}
		pos.y++;
	}
	ROOM_DEPTH = pos.y - 2;
	initState.shrink_to_fit();

	for (auto& pod : initState)
	{
		if (pod.pos.x != pod.TargetPosX())
			pod.bGoodPlace = false;
		else
		{
			pod.bGoodPlace = true;
			for (auto& otherPod : initState)
			{
				if (otherPod == pod) continue;
				if (otherPod.pos.x == pod.pos.x && otherPod.pos.y > pod.pos.y) // below
				{
					pod.bGoodPlace &= otherPod.InGoodRoom();
				}
			}
		}
	}

	Display(initState);


	auto timeDelta = clock();

	cacheDist[initState] = 0;
	queueStates.push(initState);

	unordered_set<Amphipod::Pos> posTaken;
	posTaken.reserve(11 + ROOM_DEPTH * 4 + 1);

	State currentState;
	while (!queueStates.empty())
	{
		currentState = queueStates.top();
		queueStates.pop();
		U64 uCurrentEnergy = cacheDist[currentState];

		posTaken.clear();
		for (auto& pod : currentState)
			posTaken.insert(pod.pos);

		bool bPruneAllState = false;
		for (auto& currentPod : currentState)
		{
			if (bPruneAllState)
				break;

			if (currentPod.bGoodPlace)
				continue;

			const bool bNeedsUp = currentPod.pos.y > 0;
			bool bCanGoUp = false;

			if (bNeedsUp)
			{
				bCanGoUp = true;
				Amphipod::Pos pos = currentPod.pos;
				while (pos.y > 0)
				{
					pos.y--;
					if (posTaken.find(pos) != posTaken.end())
					{
						bCanGoUp = false;
						break;
					}
				}
			}

			if (bNeedsUp && !bCanGoUp)
				continue;


			// check if it can go to its room
			// either from hallway or from an other room

			if (CheckRoomAvailable(currentState, currentPod))
			{
				if (currentPod.pos.y == 0 || bCanGoUp)
				{
					U8 targetPosX = currentPod.TargetPosX();
					if (currentPod.pos.x > targetPosX)
					{
						Amphipod::Pos testPos = { currentPod.pos.x, 0 };

						// find furthest x pos
						while (--testPos.x >= targetPosX)
						{
							if (posTaken.find(testPos) != posTaken.end())
								break;
						}
						testPos.x++;

						Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);
						if (testPos.x == targetPosX && posTaken.find(testPos) == posTaken.end())
						{
							//check depth
							while (++testPos.y <= ROOM_DEPTH)
							{
								if (posTaken.find(testPos) != posTaken.end())
									break;
							}
							testPos.y--;


							Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);

							State newState = currentState;
							auto itNewPod = find(newState.begin(), newState.end(), currentPod);
							itNewPod->pos = testPos;
							itNewPod->bGoodPlace = true;

							U64 energyMove = (glm::abs(currentPod.pos.x - targetPosX) + currentPod.pos.y + testPos.y) * currentPod.MoveCost();
							AddCandidate(newState, uCurrentEnergy + energyMove, currentState);
							bPruneAllState = true;
							continue; // no need to test the hallway positions
						}
					}
					else
					{
						Amphipod::Pos testPos = { currentPod.pos.x, 0 };

						// find furthest x pos
						while (++testPos.x <= targetPosX)
						{
							if (posTaken.find(testPos) != posTaken.end())
								break;
						}
						testPos.x--;

						Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);
						if (testPos.x == targetPosX && posTaken.find(testPos) == posTaken.end())
						{
							//check depth
							while (++testPos.y <= ROOM_DEPTH)
							{
								if (posTaken.find(testPos) != posTaken.end())
									break;
							}
							testPos.y--;



							Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);

							State newState = currentState;
							auto itNewPod = find(newState.begin(), newState.end(), currentPod);
							itNewPod->pos = testPos;
							itNewPod->bGoodPlace = true;

							U64 energyMove = (glm::abs(currentPod.pos.x - targetPosX) + currentPod.pos.y + testPos.y) * currentPod.MoveCost();
							AddCandidate(newState, uCurrentEnergy + energyMove, currentState);
							bPruneAllState = true;
							continue; // no need to test the hallway positions
						}
					}
				}
			}

			if (bCanGoUp)
			{
				// no need to add the state where we only move up

				Amphipod::Pos testPos = { currentPod.pos.x - 1, 0 };
				while (testPos.x >= 0 && posTaken.find(testPos) == posTaken.end())
				{
					if (testPos.x > 0 && testPos.x < 10 && testPos.x % 2 == 0)
					{
						testPos.x--;
						continue;
					}
					Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);

					State newState = currentState;
					auto itNewPod = find(newState.begin(), newState.end(), currentPod);
					itNewPod->pos = testPos;

					U64 energyMove = ManhatanDist(testPos, currentPod.pos) * currentPod.MoveCost();
					AddCandidate(newState, uCurrentEnergy + energyMove, currentState);
					testPos.x--;
				}

				testPos = { currentPod.pos.x + 1, 0 };
				while (testPos.x < 11 && posTaken.find(testPos) == posTaken.end())
				{
					if (testPos.x > 0 && testPos.x < 10 && testPos.x % 2 == 0)
					{
						testPos.x++;
						continue;
					}
					Assert(testPos.x >= 0 && testPos.x <= 10 && testPos.y >= 0 && testPos.y <= ROOM_DEPTH);

					State newState = currentState;
					auto itNewPod = find(newState.begin(), newState.end(), currentPod);
					itNewPod->pos = testPos;

					U64 energyMove = ManhatanDist(testPos, currentPod.pos) * currentPod.MoveCost();
					AddCandidate(newState, uCurrentEnergy + energyMove, currentState);
					testPos.x++;
				}
			}
		}

		if (best < 0xFFFFFFFFFFFFFFFF)
		{
			Display(currentState);
			State tmpState = currentState;
			auto itPrev = cachePrev.find(currentState);
			while (itPrev != cachePrev.end())
			{
				Display(itPrev->second);
				cout << endl;
				itPrev = cachePrev.find(itPrev->second);
			}


			break;
		}


		auto time = clock();
		if (time - timeDelta > CLOCKS_PER_SEC)
		{
			timeDelta = time;
			cout << queueStates.size() << " candidates";
			if (best < 0xFFFFFFFFFFFFFFFF)
				cout << " best=" << best;
			cout << endl;
		}

	}


	cout << "Best=" << best << endl;
}