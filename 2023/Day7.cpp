#include "stdafx.h"

namespace Day7
{
	constexpr int CardPower(char c, bool withJoker)
	{
		switch (c)
		{
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': return c - '0';
			case 'T': return 10;
			case 'J': return withJoker ? 1 : 11;
			case 'Q': return 12;
			case 'K': return 13;
			case 'A': return 14;
		}
		return 0;
	}

	void Solve()
	{
		struct Hand
		{
			enum class Order
			{
				NONE = -1,
				CARD,
				PAIR,
				TWO_PAIR,
				THREE,
				FULL,
				FOUR,
				FIVE
			};

			Hand(string sLine)
			{
				sHand = sLine.substr(0, sLine.find_first_of(' '));
				uBid  = stoull(sLine.substr(sLine.find_first_of(' ') + 1));


				int cardsPower[15] { 0 };
				for (int i = 0; i < 5; ++i)
				{
					cardsPower[CardPower(sHand[i], false)]++;
				}

				bool bHavePair	= false;
				bool bHaveThree = false;
				for (int p : cardsPower)
				{
					if (p == 5)
					{
						eOrder = Order::FIVE;
						break;
					}
					if (p == 4)
					{
						eOrder = Order::FOUR;
						break;
					}
					if (p == 3)
					{
						if (bHavePair)
						{
							eOrder = Order::FULL;
							break;
						}
						else
							bHaveThree = true;
					}
					if (p == 2)
					{
						if (bHaveThree)
						{
							eOrder = Order::FULL;
							break;
						}
						else if (bHavePair)
						{
							eOrder = Order::TWO_PAIR;
							break;
						}
						else
						{
							bHavePair = true;
						}
					}
				}

				if (eOrder == Order::NONE)
				{
					if (bHaveThree)
						eOrder = Order::THREE;
					else if (bHavePair)
						eOrder = Order::PAIR;
					else
						eOrder = Order::CARD;
				}

				Assert(eOrder != Order::NONE);

				// With jokers

				bHavePair						  = false;
				bHaveThree						  = false;
				nbJoker							  = cardsPower[CardPower('J', false)];
				cardsPower[CardPower('J', false)] = -1;
				for (int p : cardsPower)
				{
					if (p < 0) // joker
						continue;

					if (p == 5)
					{
						eOrderWithJoker = Order::FIVE;
						break;
					}
					if (p == 4)
					{
						eOrderWithJoker = Order::FOUR;
						break;
					}
					if (p == 3)
					{
						if (bHavePair)
						{
							eOrderWithJoker = Order::FULL;
							break;
						}
						else
							bHaveThree = true;
					}
					if (p == 2)
					{
						if (bHaveThree)
						{
							eOrderWithJoker = Order::FULL;
							break;
						}
						else if (bHavePair)
						{
							eOrderWithJoker = Order::TWO_PAIR;
							break;
						}
						else
						{
							bHavePair = true;
						}
					}
				}

				if (nbJoker > 0)
				{
					// FIVE & FULL are already taken care of

					if (eOrderWithJoker == Order::FOUR)
						eOrderWithJoker = Order::FIVE;
					else if (eOrderWithJoker == Order::TWO_PAIR)
						eOrderWithJoker = Order::FULL;
					else if (eOrderWithJoker == Order::NONE)
					{
						if (bHaveThree)
							eOrderWithJoker = nbJoker > 1 ? Order::FIVE : Order::FOUR;
						else if (bHavePair)
						{
							if (nbJoker == 3)
								eOrderWithJoker = Order::FIVE;
							else if (nbJoker == 2)
								eOrderWithJoker = Order::FOUR;
							else
								eOrderWithJoker = Order::THREE;
						}
						else
						{
							if (nbJoker >= 4)
								eOrderWithJoker = Order::FIVE;
							else if (nbJoker == 3)
								eOrderWithJoker = Order::FOUR;
							else if (nbJoker == 2)
								eOrderWithJoker = Order::THREE;
							else
								eOrderWithJoker = Order::PAIR;
						}
					}
				}
				else if (eOrderWithJoker == Order::NONE)
				{
					if (bHaveThree)
						eOrderWithJoker = Order::THREE;
					else if (bHavePair)
						eOrderWithJoker = Order::PAIR;
					else
						eOrderWithJoker = Order::CARD;
				}

				Assert(eOrderWithJoker != Order::NONE);
			}

			string sHand;
			U64	   uBid			   = 0;
			Order  eOrder		   = Order::NONE;
			Order  eOrderWithJoker = Order::NONE;
			int	   nbJoker		   = 0;

			bool operator<(const Hand &rhs)
			{
				if (eOrder != rhs.eOrder)
					return eOrder < rhs.eOrder;

				for (int i = 0; i < 5; ++i)
				{
					if (sHand[i] != rhs.sHand[i])
						return CardPower(sHand[i], false) < CardPower(rhs.sHand[i], false);
				}

				return false;
			}

			static bool lessWithJoker(const Hand &lhs, const Hand &rhs)
			{
				if (lhs.eOrderWithJoker != rhs.eOrderWithJoker)
					return lhs.eOrderWithJoker < rhs.eOrderWithJoker;

				for (int i = 0; i < 5; ++i)
				{
					if (lhs.sHand[i] != rhs.sHand[i])
						return CardPower(lhs.sHand[i], true) < CardPower(rhs.sHand[i], true);
				}
				return false;
			}
		};

		cout << "--- Day 7: Camel Cards ---" << endl;
		ifstream is("Data7.txt");

		string sLine;
		vector<Hand> vecHands;
		while (getline(is, sLine))
		{
			vecHands.emplace_back(sLine);
		}

		sort(vecHands.begin(), vecHands.end());

		U64 sum1 = 0;
		for (int i = 0; i < vecHands.size(); ++i)
		{
			sum1 += (i + 1) * vecHands[i].uBid;
		}

		sort(vecHands.begin(), vecHands.end(), &Hand::lessWithJoker);

		U64 sum2 = 0;
		for (int i = 0; i < vecHands.size(); ++i)
		{
			sum2 += (i + 1) * vecHands[i].uBid;
		}


		cout << "Part1: " << sum1 << endl;
		cout << "Part2: " << sum2 << endl;
	}
} // namespace Day7