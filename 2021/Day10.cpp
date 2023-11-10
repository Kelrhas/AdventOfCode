#include "stdafx.h"

namespace Day10
{
	void Solve()
	{
		cout << "--- Day 10: Syntax Scoring ---" << endl;
		std::ifstream is("Data10.txt");

		string sLine		 = "{([(<{}[<>[]}>{[]{[(<()>";
		sLine		 = "[({(<(())[]>[[{[]{<()<>>";
		U64	   sumCorrupted	 = 0;
		vector<U64> incompleteScores;
		while (getline(is, sLine))
		{

			// first simplify by removing every valid closing pair
			bool bChanged = false;
			do
			{
				bChanged = false;
				int i = 0;
				while (sLine[i] != '\0' && sLine[i+1] != '\0')
				{
					if (sLine[i] == '(' && sLine[i + 1] == ')' || sLine[i] == '[' && sLine[i + 1] == ']' || sLine[i] == '{' && sLine[i + 1] == '}' ||
						sLine[i] == '<' && sLine[i + 1] == '>')
					{
						// remove the two character
						sLine.erase(i, 2);
						bChanged = true;
					}
					else
						++i;
				}
			} while (bChanged);

			auto IsClosing = [](char c)
			{
				switch (c)
				{
					case ')':
					case ']':
					case '}':
					case '>': return true;
				}
				return false;
			};
			auto CorruptedScore = [](char c)
			{
				switch (c)
				{
					case ')': return 3;
					case ']': return 57;
					case '}': return 1197;
					case '>': return 25137;
				}
				return 0;
			};


			bool bCorrupted = false;
			for (char c : sLine)
			{
				if (IsClosing(c))
				{
					sumCorrupted += CorruptedScore(c);
					bCorrupted = true;
					break;
				}
			}

			if (bCorrupted)
				continue;

			auto Match = [](char c)
			{
				switch (c)
				{
					case '(': return ')';
					case '[': return ']';
					case '{': return '}';
					case '<': return '>';
				}
				return '0';
			};
			auto IncompleteScore = [](char c)
			{
				switch (c)
				{
					case ')': return 1;
					case ']': return 2;
					case '}': return 3;
					case '>': return 4;
				}
				return 0;
			};

			U64 score = 0;
			// add from the end !!
			for (auto it = sLine.rbegin(); it != sLine.rend(); ++it)
			{
				score *= 5;
				score += IncompleteScore(Match(*it));
			}
			incompleteScores.emplace_back(score);
		}

		sort(incompleteScores.begin(), incompleteScores.end());


		cout << "Part1: " << sumCorrupted << endl;
		cout << "Part2: " << incompleteScores[incompleteScores.size()/2] << endl;
	}
} // namespace Day10