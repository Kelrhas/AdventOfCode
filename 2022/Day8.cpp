#include "stdafx.h"

namespace Day8
{
	void Solve()
	{
		cout << "--- Day 8: Treetop Tree House ---" << endl;
		std::ifstream is("Data8.txt");

		vector<vector<int>> trees;

		string line;
		int	   row = 0;
		while (getline(is, line))
		{
			size_t firstSize = trees.empty() ? -1 : trees.front().size();
			trees.emplace_back();
			trees.back().reserve(line.size());
			for (char c : line)
				trees.back().push_back(c - '0');

			if (firstSize != -1)
				assert(trees.back().size() == firstSize);
		}

		const int GRID_HEIGHT = trees.size();
		const int GRID_WIDTH  = trees[0].size();

		int sum = GRID_HEIGHT * 2 + GRID_WIDTH * 2 - 4;


		unordered_set<pair<int, int>, pair_hash> visible;

		for (int h = 1; h < GRID_HEIGHT - 1; ++h)
		{
			int previous = trees[h][0];
			for (int w = 1; w < GRID_WIDTH - 1; ++w)
			{
				if (trees[h][w] > previous)
				{
					visible.emplace(h, w);
					previous = trees[h][w];
				}
			}

			previous = trees[h][GRID_WIDTH - 1];
			for (int w = GRID_WIDTH - 2; w >= 1; --w)
			{
				if (trees[h][w] > previous)
				{
					visible.emplace(h, w);
					previous = trees[h][w];
				}
			}
		}

		for (int w = 1; w < GRID_WIDTH - 1; ++w)
		{
			int previous = trees[0][w];
			for (int h = 1; h < GRID_HEIGHT - 1; ++h)
			{
				if (trees[h][w] > previous)
				{
					visible.emplace(h, w);
					previous = trees[h][w];
				}
			}

			previous = trees[GRID_HEIGHT - 1][w];
			for (int h = GRID_HEIGHT - 2; h >= 1; --h)
			{
				if (trees[h][w] > previous)
				{
					visible.emplace(h, w);
					previous = trees[h][w];
				}
			}
		}


		cout << "Part1:" << sum + visible.size() << endl;

		unordered_map<pair<int, int>, int, pair_hash> scores;
		for (int h = 1; h < GRID_HEIGHT - 1; ++h)
		{
			for (int w = 1; w < GRID_WIDTH - 1; ++w)
			{
				int current = trees[h][w];
				// left
				int countLeft = 0;
				for (int i = w - 1; i >= 0; --i)
				{
					countLeft++;
					if (trees[h][i] >= current)
						break;
				}
				// right
				int countRight = 0;
				for (int i = w + 1; i < GRID_WIDTH; ++i)
				{
					countRight++;
					if (trees[h][i] >= current)
						break;
				}
				// top
				int countTop = 0;
				for (int i = h - 1; i >= 0; --i)
				{
					countTop++;
					if (trees[i][w] >= current)
						break;
				}
				// bottom
				int countBottom = 0;
				for (int i = h + 1; i < GRID_HEIGHT; ++i)
				{
					countBottom++;
					if (trees[i][w] >= current)
						break;
				}

				scores.emplace(pair<int, int> { h, w }, countLeft * countRight * countTop * countBottom);
			}
		}


		int highest = 0;
		for (const auto &[key, value] : scores)
		{
			highest = max(highest, value);
		}


		cout << "Part2:" << highest << endl;
	}
} // namespace Day8