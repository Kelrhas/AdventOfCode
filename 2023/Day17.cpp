#include "stdafx.h"

namespace Day17
{
	enum Direction
	{
		RIGHT,
		DOWN,
		LEFT,
		UP
	};
	constexpr glm::ivec2 GetDirectionVec(Direction dir)
	{
		if (dir == RIGHT)
			return glm::ivec2 { 1, 0 };
		if (dir == DOWN)
			return glm::ivec2 { 0, 1 };
		if (dir == LEFT)
			return glm::ivec2 { -1, 0 };

		// UP
		return glm::ivec2 { 0, -1 };
	}
	struct State
	{
		glm::ivec2 pos { 0 };
		U8		   straightStreak = 0;
		Direction  direction	  = RIGHT; // for straightstreak

		bool operator==(const State &rhs) const { return pos == rhs.pos && straightStreak == rhs.straightStreak && direction == rhs.direction; }
	};

} // namespace Day17


template<>
struct hash<Day17::State>
{
	size_t operator()(const Day17::State &s) const
	{
		size_t h = hash<glm::ivec2>()(s.pos);
		h ^= (size_t)(s.straightStreak << 2 | s.direction) << 7;
		return h;
	}
};

namespace Day17
{
	void Solve()
	{
		cout << "--- Day 17: Clumsy Crucible ---" << endl;
		ifstream is("Data17.txt");

		vector<vector<U8>> input;

#if 0
		for (string sLine : {
			   "2413432311323",
			   "3215453535623",
			   "3255245654254",
			   "3446585845452",
			   "4546657867536",
			   "1438598798454",
			   "4457876987766",
			   "3637877979653",
			   "4654967986887",
			   "4564679986453",
			   "1224686865563",
			   "2546548887735",
			   "4322674655533",
			 })
#else
		string sLine;
		while (getline(is, sLine))
#endif
		{
			input.emplace_back();
			for (char c : sLine)
				input.back().emplace_back(c - '0');
		}

		const int		 WIDTH	= input.front().size();
		const int		 HEIGHT = input.size();
		const glm::ivec2 vEndPos { WIDTH - 1, HEIGHT - 1 };


		struct PriorityCompare
		{
			bool operator()(const pair<State, U32> &lhs, const pair<State, U32> &rhs) const { return lhs.second > rhs.second; }
		};

		// we want to minimize the score
		unordered_map<State, U32> cache;
		using QueueElmt = pair<State, U32>;
		vector<QueueElmt> vecQueue;
		vecQueue.reserve(1 << 12);
		priority_queue<QueueElmt, decltype(vecQueue), PriorityCompare> pending(PriorityCompare {}, std::move(vecQueue)); // state & score
		cache.reserve(1 << 12);

		auto Part = [&](int minStreak, int maxStreak)
		{
			cache.clear();

			U32	 uMaxScore = -1;
			bool debug	   = false;

			State startRight;
			startRight.direction = RIGHT;
			startRight.pos		 = { 1, 0 };
			cache[startRight]	 = input[0][1];
			pending.emplace(startRight, input[0][1]);

			State startDown;
			startDown.direction = DOWN;
			startDown.pos		= { 0, 1 };
			cache[startDown]	= input[1][0];
			pending.emplace(startDown, input[1][0]);


			auto TestAndAdd = [&](State newState, U32 curScore)
			{
				if (newState.pos.x >= 0 && newState.pos.x < WIDTH && newState.pos.y >= 0 && newState.pos.y < HEIGHT)
				{
					U32 uNewScore = curScore + input[newState.pos.y][newState.pos.x];
					if (uNewScore < uMaxScore)
					{
						auto [it, inserted] = cache.insert({ newState, uNewScore });
						if (inserted)
							pending.push({ newState, uNewScore });
					}
				}
			};


			while (!pending.empty())
			{
				const auto [curState, curScore] = pending.top();

				if (debug)
					cout << "Trying state at pos=" << curState.pos.x << "," << curState.pos.y << " dir=" << curState.direction << " score=" << curScore << endl;

				if (curState.pos == vEndPos && curState.straightStreak >= minStreak - 1)
				{
					if (debug)
						cout << "Reached end with score=" << curScore << endl;
					uMaxScore = min(uMaxScore, curScore);
					pending.pop();
					continue;
				}

				// try forward
				if (curState.straightStreak < maxStreak - 1)
				{
					State newState = curState;
					newState.straightStreak++;
					newState.pos += GetDirectionVec(newState.direction);
					TestAndAdd(newState, curScore);
				}

				// try turning left
				if (curState.straightStreak >= minStreak - 1)
				{
					State newState			= curState;
					newState.direction		= (Direction)((newState.direction + 3) % 4);
					newState.straightStreak = 0;
					newState.pos += GetDirectionVec(newState.direction);
					TestAndAdd(newState, curScore);
				}

				// try turning right
				if (curState.straightStreak >= minStreak - 1)
				{
					State newState			= curState;
					newState.direction		= (Direction)((newState.direction + 1) % 4);
					newState.straightStreak = 0;
					newState.pos += GetDirectionVec(newState.direction);
					TestAndAdd(newState, curScore);
				}

				pending.pop();
			}

			return uMaxScore;
		};

		cout << "Part1: " << Part(1, 3) << endl;
		cout << "Part2: " << Part(4, 10) << endl;

#pragma endregion
	}
} // namespace Day17