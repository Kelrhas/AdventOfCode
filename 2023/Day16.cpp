#include "stdafx.h"

namespace Day16
{
	constexpr glm::ivec2 vLeft { -1, 0 };
	constexpr glm::ivec2 vRight { 1, 0 };
	constexpr glm::ivec2 vUp { 0, -1 };
	constexpr glm::ivec2 vDown { 0, 1 };

	struct PosAndDir
	{
		glm::ivec2 vPos;
		glm::ivec2 vDir;
		size_t	   uHash = 0;

		PosAndDir(const glm::ivec2 &pos, const glm::ivec2 &dir) : vPos(pos), vDir(dir) { uHash = hash<glm::ivec2>()(vPos) ^ hash<glm::ivec2>()(vDir); }

		bool operator==(const PosAndDir &rhs) const { return uHash == rhs.uHash && vPos == rhs.vPos && vDir == rhs.vDir; }
	};
} // namespace Day16

template<>
struct hash<Day16::PosAndDir>
{
	size_t operator()(const Day16::PosAndDir &pad) const { return pad.uHash; }
};

namespace Day16
{
	void Solve()
	{
		cout << "--- Day 16: The Floor Will Be Lava ---" << endl;
		ifstream is("Data16.txt");

		vector<vector<char>> input;

#if 0
		for (string sLine : { R"(.|...\....)",
							  R"(|.-.\.....)",
							  R"(.....|-...)",
							  R"(........|.)",
							  R"(..........)",
							  R"(.........\)",
							  R"(..../.\\..)",
							  R"(.-.-/..|..)",
							  R"(.|....-|.\)",
							  R"(..//.|....)" })
#else
		string sLine;
		while (getline(is, sLine))
#endif
		{
			input.emplace_back(sLine.begin(), sLine.end());
		}

		const int WIDTH	 = input.front().size();
		const int HEIGHT = input.size();

		unordered_set<glm::ivec2> energized;
		unordered_set<PosAndDir>  cache;
		queue<PosAndDir>		  pending;

		auto InsertPending = [&](PosAndDir pad)
		{
			auto it = cache.find(pad);
			if (it == cache.end())
			{
				pending.emplace(pad);
			}
		};

		auto ComputeEnergy = [&]() -> U64
		{
			while (!pending.empty())
			{
				auto &[vPos, vDir, h] = pending.front();

				while (vPos.x >= 0 && vPos.x < WIDTH && vPos.y >= 0 && vPos.y < HEIGHT)
				{
					auto [it, inserted] = cache.emplace(vPos, vDir);
					if(!inserted)
						break; // already treated this one

					energized.emplace(vPos);
					char c = input[vPos.y][vPos.x];

					// update direction & pending
					switch (c)
					{
						case '.': break; // same direction
						case '-':
						{
							if (vDir.y != 0)
							{
								// split the beam, one pending & continue the other
								InsertPending({ vPos + vLeft, vLeft });
								vDir = vRight;
							}
							break;
						}
						case '|':
						{
							if (vDir.x != 0)
							{
								// split the beam, one pending & continue the other
								InsertPending({ vPos + vUp, vUp });
								vDir = vDown;
							}
							break;
						}
						case '/':
						{
							if (vDir.x < 0)
								vDir = vDown;
							else if (vDir.x > 0)
								vDir = vUp;
							else if (vDir.y < 0)
								vDir = vRight;
							else if (vDir.y > 0)
								vDir = vLeft;

							break;
						}
						case '\\':
						{
							if (vDir.x < 0)
								vDir = vUp;
							else if (vDir.x > 0)
								vDir = vDown;
							else if (vDir.y < 0)
								vDir = vLeft;
							else if (vDir.y > 0)
								vDir = vRight;

							break;
						}
					}
					vPos += vDir;
				}

				pending.pop();
			}

			U64 energy = energized.size();
			energized.clear();
			cache.clear();
			return energy;
		};

		//reserve maximum memory
		energized.reserve(WIDTH * HEIGHT);
		cache.reserve(WIDTH * HEIGHT);

		pending.emplace(glm::ivec2 { 0, 0 }, vRight);
		U64 uSum1 = ComputeEnergy();

		pending.emplace(glm::ivec2 { WIDTH - 1, 0 }, vLeft);
		U64 uSum2 = max(uSum1, ComputeEnergy());

		/**/
		for (int y = 1; y < HEIGHT; ++y)
		{
			pending.emplace(glm::ivec2 { 0, y }, vRight);
			uSum2 = max(uSum2, ComputeEnergy());
			pending.emplace(glm::ivec2 { WIDTH - 1, y }, vLeft);
			uSum2 = max(uSum2, ComputeEnergy());
		}

		for (int x = 0; x < WIDTH; ++x)
		{
			pending.emplace(glm::ivec2 { x, 0 }, vDown);
			uSum2 = max(uSum2, ComputeEnergy());
			pending.emplace(glm::ivec2 { x, HEIGHT - 1 }, vUp);
			uSum2 = max(uSum2, ComputeEnergy());
		}
		/**/

		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << uSum2 << endl;
	}
} // namespace Day16