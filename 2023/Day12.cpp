#include "stdafx.h"

namespace Day12
{
	struct State
	{
		string str;
	};

	vector<int> GroupsByString(string_view sv)
	{
		vector<int> vec;
		bool		bInGroup = false;

		while (!sv.empty())
		{
			size_t unknown = sv.find_first_of('?');
			size_t beg	   = sv.find_first_of('#');		 // find where the # starts
			size_t off	   = sv.find_first_of('.', beg); // find where the # surely stops

			if (beg == string::npos || // not found
				unknown < beg)		   // ? before #
			{
				break;
			}

			if (off == string::npos)
			{
				off = sv.size();
				sv	= {};
			}
			else
				sv = sv.substr(off + 1);

			size_t offMin = min(unknown, off);

			vec.emplace_back(offMin - beg);

			if (unknown < off) // temporary group, dont go further
				break;
		}
		return vec;
	}

	void Solve()
	{
		cout << "--- Day 12: Hot Springs ---" << endl;
		ifstream is("Data12.txt");

		string		 sLine;
		queue<State> pending;
		U64			 sum1 = 0;
		while (getline(is, sLine))
		{
			string_view svLine	  = sLine;
			string_view svSprings = svLine.substr(0, svLine.find_first_of(' '));
			string_view svNums	  = svLine.substr(svLine.find_first_of(' ') + 1);

			vector<int>		   vecNumsInput;
			std::istringstream ss((string)svNums);
			string			   sTmp;
			while (getline(ss, sTmp, ','))
			{
				vecNumsInput.emplace_back(stoi(sTmp));
			}

			U32 uNbCase = 0;


			bool bDebug = false;
			if (bDebug && sLine.empty())
				break;

			if (bDebug)
				cout << sLine << endl;

			// GroupsByString("####.#...#...");

			// start the queue
			pending.emplace(State { .str = (string)svSprings });
			while (!pending.empty())
			{
				State &curState = pending.front();
				size_t size		= curState.str.size();

				for (int iChar = 0; iChar < size; ++iChar)
				{
					const char c = curState.str[iChar];
					if (c == '?')
					{
						bool bFinished = true;
						for (int iRemain = iChar + 1; iRemain < size; ++iRemain)
						{
							if (curState.str[iRemain] == '?')
							{
								bFinished = false;
							}
						}

						auto NewState = [&](char c)
						{
							State newState		  = curState;
							newState.str[iChar]	  = c;
							const auto &vecGroups = GroupsByString(newState.str);
							bool		bGood	  = !bFinished || vecGroups.size() == vecNumsInput.size();
							if (vecGroups.size() <= vecNumsInput.size())
							{
								for (int iGroup = 0; iGroup < vecGroups.size(); iGroup++)
								{
									int found = vecGroups[iGroup];
									int input = vecNumsInput[iGroup];

									if (found > input) // group is too large
										bGood = false;

									if (found != input && bFinished)
										bGood = false;
								}
							}
							else
								bGood = false;

							if (bGood)
							{
								if (bFinished)
								{
									if (bDebug)
									{
										SetConsoleTextAttribute(hConsole, CONSOLE_GREEN);
										cout << '\t' << newState.str << endl;
										SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
									}
									uNbCase++;
								}
								else
								{
									if (bDebug)
										cout << '\t' << newState.str << endl;
									pending.emplace(std::move(newState));
								}
							}
							else if (bDebug)
							{
								SetConsoleTextAttribute(hConsole, CONSOLE_RED);
								cout << '\t' << newState.str << "  ";
								for (int i : vecGroups)
									cout << i << ' ';
								cout << endl;
								SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
							}
						};

						NewState('#');
						NewState('.');

						break;
					}
				}

				pending.pop();
			}

			if (bDebug)
				cout << " => " << uNbCase << endl;
			sum1 += uNbCase;
		}

		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << endl;
	}
} // namespace Day12