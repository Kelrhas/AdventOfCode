#include "stdafx.h"

namespace Day8
{
	void Solve()
	{
		cout << "--- Day 8: Seven Segment Search ---" << endl;
		std::ifstream is("Data8.txt");

		/*
		* 
			 aaaa            aaaa    aaaa        	  aaaa    aaaa    aaaa    aaaa    aaaa
			b    c       c       c       c  b    c	 b       b            c  b    c  b    c
			b    c       c       c       c  b    c	 b       b            c  b    c  b    c
			                 dddd    dddd    dddd	  dddd    dddd            dddd    dddd
			e    f       f  e            f       f	      f  e    f       f  e    f       f
			e    f       f  e            f       f	      f  e    f       f  e    f       f
			 gggg            gggg    gggg        	  gggg    gggg            gggg    gggg

		 2 segments => 1
		 3 segments => 7
		 4 segments => 4
		 5 segments => 2 3 5
		 6 segments => 0 6 9
		 7 segments => 8

		 A used 8 times
		 B used 6 times
		 C used 8 times
		 D used 7 times
		 E used 4 times
		 F used 9 times
		 G used 7 times

		 Strategy:
		 - find segment A with 7 - 1
		 - find segment B with occurrences
		 - find segment E with occurrences
		 - find segment F with occurrences
		 - find segment C with occurrences (not A)
		 - find segment D with 4 - 1 - B
		 - find segment G with occurrences (not D)
		 */


		struct Digit
		{
			bool segment[7] = { false };

			Digit() = default;
			Digit(const Digit &_rhs) { *this = _rhs; }
			Digit(Digit &&_rhs) { *this = std::move(_rhs); }
			Digit(char c)
			{
				if (c >= 'a' && c <= 'g')
				{
					segment[c - 'a'] = true;
				}
			}
			Digit(const string& str)
			{
				for (char c : str)
				{
					if (c >= 'a' && c <= 'g')
					{
						segment[c - 'a'] = true;
					}
				}
			}
			Digit(const string& str, int mapping[7])
			{
				for (char c : str)
				{
					if (c >= 'a' && c <= 'g')
					{
						segment[mapping[c - 'a']] = true;
					}
				}
			}
			Digit(int i)
			{
				switch (i)
				{
					case 0:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 3;
						break;
					case 1: segment[2] = segment[5] = true;
						break;
					case 2:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 1 && i != 5;
						break;
					case 3:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 1 && i != 4;
						break;
					case 4: segment[1] = segment[2] = segment[3] = segment[5] = true;
						break;
					case 5:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 2 && i != 4;
						break;
					case 6:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 2;
						break;
					case 7: segment[0] = segment[2] = segment[5] = true;
						break;
					case 8:
						for (int i = 0; i < 7; ++i)
							segment[i] = true;
						break;
					case 9:
						for (int i = 0; i < 7; ++i)
							segment[i] = i != 4;
						break;
				}
			}

			bool operator==(const Digit& _rhs) const
			{
				for (int i = 0; i < 7; ++i)
				{
					if (segment[i] != _rhs.segment[i])
						return false;
				}
				return true;
			}
			Digit& operator = (const Digit &_rhs)
			{
				for (int i = 0; i < 7; ++i)
					segment[i] = _rhs.segment[i];
				return *this;
			}
			Digit& operator = (Digit &&_rhs)
			{
				for (int i = 0; i < 7; ++i)
					segment[i] = _rhs.segment[i];
				return *this;
			}
			Digit operator + (const Digit &_rhs)
			{
				Digit res = *this;
				for (int i = 0; i < 7; ++i)
					res.segment[i] |= _rhs.segment[i];
				return res;
			}
			Digit operator - (const Digit &_rhs)
			{
				Digit res = *this;
				for (int i = 0; i < 7; ++i)
					res.segment[i] &= !_rhs.segment[i];
				return res;
			}

			void Display() const
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);

				if(segment[0])
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << "      ";
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << endl;

				for (int y = 0; y < 2; ++y)
				{
					if (segment[1])
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << " ";
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << "    ";
					if (segment[2])
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << " ";
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << endl;
				}

				if (segment[3])
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << "      ";
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << endl;

				for (int y = 0; y < 2; ++y)
				{
					if (segment[4])
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << " ";
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << "    ";
					if (segment[5])
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << " ";
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
					cout << endl;
				}

				if (segment[6])
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE_BACKGROUND);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << "      ";
				SetConsoleTextAttribute(hConsole, CONSOLE_WHITE);
				cout << endl;
			}

		};

		U64	   sum = 0;
		U64	   easyDigitOccurences = 0;
		string sLine			   = "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf";
		while (getline(is, sLine))
		{
			int						 mappingWrongToRight[7];
			int						 mappingRightToWrong[7];
			memset(mappingWrongToRight, -1, sizeof(int) * 7);
			memset(mappingRightToWrong, -1, sizeof(int) * 7);
			vector<string>			 words;
			unordered_map<char, int> occurrences;
			for (const auto &s : sLine | views::split(' ') | views::take(10))
			{
				string str(s.begin(), s.end());
				transform(str.begin(), str.end(), str.begin(), tolower);
				words.emplace_back(str);
				for (char c : s)
					occurrences[c]++;
			}

			// sort by size
			sort(words.begin(), words.end(), [](const string &w1, const string &w2) { return w1.size() < w2.size(); });

			// find segment A with 7 - 1
			for (char c : words[1])
			{
				if (words[0][0] != c && words[0][1] != c)
				{
					mappingWrongToRight[c - 'a'] = 0;
					mappingRightToWrong[0]		 = c - 'a';
					break;
				}
			}
			
			for (const auto &[c, occ] : occurrences)
			{
				// find segment B with occurrences
				if (occ == 6)
				{
					mappingWrongToRight[c - 'a'] = 1;
					mappingRightToWrong[1]		 = c - 'a';
				}
				// find segment E with occurrences
				else if (occ == 4)
				{
					mappingWrongToRight[c - 'a'] = 4;
					mappingRightToWrong[4]		 = c - 'a';
				}
				// find segment F with occurrences
				else if (occ == 9)
				{
					mappingWrongToRight[c - 'a'] = 5;
					mappingRightToWrong[5]		 = c - 'a';
				}
				// find segment C with occurrences (not A)
				else if (occ == 8)
				{
					if (mappingWrongToRight[c - 'a'] == -1)
					{
						mappingWrongToRight[c - 'a'] = 2;
						mappingRightToWrong[2]		 = c - 'a';
					}
				}
			}
			// find segment D with 4 - 1 - B
			{
				Digit four(words[2]);
				Digit one(words[0]);
				Digit B(char('a' + mappingRightToWrong[1]));

				Digit res = four - one - B;
				for (int i = 0; i < 7; ++i)
				{
					if (res.segment[i])
					{
						mappingWrongToRight[i] = 3;
						mappingRightToWrong[3] = i;
					}
				}
			}

			for (const auto &[c, occ] : occurrences)
			{
				// find segment G with occurrences (not D)
				if (occ == 7)
				{
					if (mappingWrongToRight[c - 'a'] == -1)
					{
						mappingWrongToRight[c - 'a'] = 6;
						mappingRightToWrong[6]		 = c - 'a';
						break;
					}
				}
			}

			
			// check the number now that we have the mapping
			U64 number = 0;
			for (const auto &s : sLine | views::split(' ') | views::drop(11))
			{
				string str(s.begin(), s.end());
				transform(str.begin(), str.end(), str.begin(), tolower);

				if (str.size() == 2 || str.size() == 3 || str.size() == 4 || str.size() == 7)
					easyDigitOccurences++;


				number *= 10;
				Digit digitWrong(str);
				Digit digitMapped(str, mappingWrongToRight);
				for (int i = 1; i < 10; ++i)
				{
					Digit digit(i);
					if (digit == digitMapped)
					{
						number += i;
						break;
					}
				}

			}

			sum += number;
		}

		cout << "Part1: " << easyDigitOccurences << endl;
		cout << "Part2: " << sum << endl;
	}
} // namespace Day8