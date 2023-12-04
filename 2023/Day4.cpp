#include "stdafx.h"

namespace Day4
{
	void Solve()
	{
		cout << "--- Day 4: Scratchcards ---" << endl;
		ifstream is("Data4.txt");
		string	 sLine;
		int		 sum1 = 0;
		struct Card
		{
			int num;
			int winningNumbers;
			int cachedTotal = -1;
		};
		vector<Card> cards;
		while (getline(is, sLine))
		{
			auto		pipe = sLine.find_first_of('|');
			string_view svWinning { sLine.begin() + sLine.find_first_of(':') + 1, sLine.begin() + pipe };
			string_view svNumbers { sLine.begin() + pipe+1, sLine.end() };

			vector<int>	 vecWinning;
			stringstream ssW { string(svWinning) };
			int			 numTmp;
			while (ssW >> numTmp)
				vecWinning.emplace_back(numTmp);

			vector<int>	 vecNumbers;
			stringstream ssN { string(svNumbers) };
			while (ssN >> numTmp)
				vecNumbers.emplace_back(numTmp);

			

			int numWinningNumber = 0;
			for (int num : vecNumbers)
			{
				if (auto it = find(vecWinning.begin(), vecWinning.end(), num); it != vecWinning.end())
				{
					numWinningNumber++;
					vecWinning.erase(it);
					if (vecWinning.empty())
						break;
				}
			}

			if (numWinningNumber > 0)
			{
				sum1 += 1 << (numWinningNumber - 1);
			}
			cards.emplace_back((int)cards.size(), numWinningNumber);
		}
		cout << "Part1: " << sum1 << endl;


		auto Compute = [&](const auto &self, Card &card, int index) -> int {

			if (card.cachedTotal >= 0)
				return card.cachedTotal;

			card.cachedTotal = card.winningNumbers;
			for (int i = 0; i < card.winningNumbers; ++i)
			{
				card.cachedTotal += self(self, cards[index + 1 + i], index + 1 + i);
			}

			return card.cachedTotal;
		};

		size_t sum2 = cards.size();
		for (int i=0; i < cards.size(); ++i)
		{
			auto &card = cards[i];
			sum2 += Compute(Compute, card, i);
		}
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day4