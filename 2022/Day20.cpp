#include "stdafx.h"

namespace Day20
{
	void Solve()
	{
		cout << "--- Day 20: Grove Positioning System ---" << endl;
		ifstream is("Data20.txt");
		string	 line;

		CircleList<S64> circle;
		while (getline(is, line))
		{
			stringstream ss(line);
			int			 i;
			ss >> i;
			circle.Push(i);
		}


		auto Run = [=](const CircleList<S64> &circleCopy, int nbRoll, size_t multiplier) {
			CircleList<S64> circle	 = circleCopy;
			S64	 NB_INPUT = (int)circle.size();
			bool debug	  = false;
			if (debug)
			{
				cout << "Init:" << endl;
				auto it = circle.begin();
				for (size_t i = 0; i < circle.size(); ++i, ++it)
					cout << *it << ", ";
				cout << endl << endl;
			}

			auto it = circle.begin();
			for (size_t i = 0; i < circle.size(); ++i, ++it)
				*it *= multiplier;

			// keep track of the positionning
			vector<CircleList<S64>::iterator> origList;
			{
				auto it = circle.begin();
				for (size_t i = 0; i < circle.size(); ++i)
				{
					origList.push_back(it++);
				}
			}

			for (int roll = 0; roll < nbRoll; ++roll)
			{
				for (auto &it : origList)
				{
					auto Mod = [](S64 a, S64 b) -> S64 {
						return a - b * S64(a / b);
					};

					// find shortest path
					S64 step = Mod(*it, NB_INPUT - 1);
					if (Mod(glm::abs(*it), NB_INPUT - 1) < step)
						step = -Mod(glm::abs(*it), NB_INPUT - 1);

					// step = *it;

					if (debug)
						cout << "\t" << *it << " -> " << step << " steps" << endl;

					auto itSwap = it;

					if (step == 0)
						continue;

					if (step < 0)
						step--;

					for (int i = 0; i < glm::abs(step); ++i)
					{
						if (step > 0)
						{
							++itSwap;
						}
						else
						{
							--itSwap;
						}
					}
					circle.MoveAfter(it, itSwap);
				}

				if (debug)
				{
					auto it = circle.begin();
					for (size_t i = 0; i < circle.size(); ++i, ++it)
						cout << *it << ", ";
					cout << endl;
				}
			}
			/*
			 auto itOrig	   = lCoord.begin();
			 auto itCurrent = lWork.begin();
			 while (itOrig != lCoord.end())
			{
				if (debug)
				{
					for (int i : lWork)
						cout << i << ", ";
					cout << endl;
				}

				// find shortest path
				int step = *itOrig % NB_INPUT;
				if ((glm::abs(*itOrig) % NB_INPUT) < step)
					step = -(int)(glm::abs(*itOrig) % NB_INPUT);

				if (debug)
					cout << "\t" << *itOrig << " -> " << step << " steps" << endl;

				auto itSwap = itCurrent;
				itOrig++;

				if (step == 0)
					continue;

				if (step > 0) // for list::insert
					step++;

				for (int i = 0; i < glm::abs(step); ++i)
				{
					if (step > 0)
					{
						itSwap++;
						if (itSwap == lWork.end())
							itSwap = lWork.begin(); // wrap around
					}
					else
					{
						if (itSwap == lWork.begin())
							itSwap = lWork.end(); // wrap around
						itSwap--;
					}

					// if (step > 0)
					//{
					//	auto itPos = itSwap++;
					//	if (itSwap == lWork.end())
					//		itSwap = lWork.begin(); // wrap around
					//	std::swap(*itPos, *itSwap);
					//}
					// else
					//{
					//	itSwap	   = lWork.begin();
					//	auto itPos = itSwap--;
					//	if (itSwap == lWork.end())
					//		itSwap = lWork.begin(); // wrap around
					//	std::swap(*itPos, *itSwap);
					//}
				}
				lWork.insert(itSwap, *itCurrent);
				itCurrent = lWork.erase(itCurrent);
			}


			// find solution

			int idx1000 = 1000 % NB_INPUT;
			int idx2000 = 2000 % NB_INPUT;
			int idx3000 = 3000 % NB_INPUT;
			int coord1000;
			int coord2000;
			int coord3000;
			int idx0 = 0;
			for (auto it = lWork.begin(); it != lWork.end(); ++it)
			{
				if (*it == 0)
				{
					auto itCoord = it;
					for (int i = 0; i < idx1000; ++i)
					{
						itCoord++;
						if (itCoord == lWork.end())
							itCoord = lWork.begin();
					}
					coord1000 = *itCoord;
					itCoord	  = it;
					for (int i = 0; i < idx2000; ++i)
					{
						itCoord++;
						if (itCoord == lWork.end())
							itCoord = lWork.begin();
					}
					coord2000 = *itCoord;
					itCoord	  = it;
					for (int i = 0; i < idx3000; ++i)
					{
						itCoord++;
						if (itCoord == lWork.end())
							itCoord = lWork.begin();
					}
					coord3000 = *itCoord;
					break;
				}
				else
				{
					idx0++;
				}
			}
			*/


			int idx1000 = 1000 % NB_INPUT;
			int idx2000 = 2000 % NB_INPUT;
			while (idx2000 < idx1000)
				idx2000 += NB_INPUT;
			int idx3000 = 3000 % NB_INPUT;
			while (idx3000 < idx2000)
				idx3000 += NB_INPUT;
			S64 coord1000 = 0;
			S64 coord2000 = 0;
			S64 coord3000 = 0;
			int idx0	  = 0;

			auto itCircle = circle.begin();
			for (size_t i = 0; i < circle.size(); ++i, ++itCircle)
			{
				if (*itCircle == 0)
				{
					auto itCoord = itCircle;
					for (int i = 0; i < idx1000; ++i)
					{
						++itCoord;
					}
					coord1000 = *itCoord;
					for (int i = idx1000; i < idx2000; ++i)
					{
						++itCoord;
					}
					coord2000 = *itCoord;
					for (int i = idx2000; i < idx3000; ++i)
					{
						++itCoord;
					}
					coord3000 = *itCoord;
					break;
				}
				else
				{
					idx0++;
				}
			}

			return coord1000 + coord2000 + coord3000;
		};

		cout << "Part1:" << Run(circle, 1, 1) << endl;
		cout << "Part2:" << Run(circle, 10, 811589153llu) << endl;
	}
} // namespace Day20