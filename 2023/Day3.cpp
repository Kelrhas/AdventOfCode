#include "stdafx.h"

namespace Day3
{
	void Solve()
	{
		cout << "--- Day 3: Gear Ratios ---" << endl;
		ifstream is("Data3.txt");

		struct Number
		{
			glm::ivec2 vStart;
			int		   iLength;
			int		   iValue;
			bool	   bAdjacentSymbol = false;
		};
		struct Symbol
		{
			glm::ivec2 vPos;
			char	   c;
			int		   iGearRatio  = 0;
		};

		vector<Number> vecNumbers;
		vector<Symbol> vecSymbols;

		string sLine;
		int	   iLine = 0;
		while (getline(is, sLine))
		{
			std::replace(sLine.begin(), sLine.end(), '.', ' ');
			for (auto it = sLine.begin(); it != sLine.end(); ++it)
			{
				char	  c		= *it;
				const int iChar = distance(sLine.begin(), it);

				if (isdigit(c))
				{
					// first digit of a number
					string_view sv { it, sLine.end() };
					int			value;
					auto		res	   = from_chars(sv.data(), sv.data() + sv.size(), value);
					int			length = res.ptr - sv.data();
					it += length - 1; // advance to next char to read


					vecNumbers.emplace_back(glm::ivec2 { iChar, iLine }, length, value);
				}
				else if (!isspace(c))
				{
					vecSymbols.emplace_back(glm::ivec2 { iChar, iLine }, c);
				}
			}
			++iLine;
		}

		vector<int> vecAdjacent;
		for (Symbol &sym : vecSymbols)
		{
			vecAdjacent.clear();
			for (Number &num : vecNumbers)
			{
				for (int i = 0; i < num.iLength && !num.bAdjacentSymbol; ++i)
				{
					glm::ivec2 vNumPos { num.vStart.x + i, num.vStart.y };

					if (ChebyshevDist(sym.vPos, vNumPos) <= 1)
					{
						num.bAdjacentSymbol = true;
						vecAdjacent.emplace_back(num.iValue);
						break;
					}
				}
			}

			if (vecAdjacent.size() >= 2)
			{
				sym.iGearRatio = accumulate(vecAdjacent.begin(), vecAdjacent.end(), 1, std::multiplies<int>{});
			}
		}

		int sum1 = 0;
		for(const auto& num : vecNumbers)
		{
			if (num.bAdjacentSymbol)
				sum1 += num.iValue;
		}

		int sum2 = 0;
		for (const auto &sym : vecSymbols)
		{
			sum2 += sym.iGearRatio;
		}

		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day3