#include "stdafx.h"

namespace Day9
{
	void Solve()
	{
		cout << "--- Day 9:  ---" << endl;
		ifstream is("Data9.txt");

		string sLine;
		vector<int> vecNum, vecWorking, vecLastNum, vecFirstNum;
		S64			iSum1 = 0;
		S64			iSum2 = 0;
		while (getline(is, sLine))
		{
			vecNum.clear();
			vecWorking.clear();
			vecLastNum.clear();
			vecFirstNum.clear();

			std::istringstream ss(sLine);
			vecNum.assign(std::istream_iterator<int>(ss), std::istream_iterator<int>());

			// derivative stuff
			bool bAllZero = false;

			while (!bAllZero)
			{
				bAllZero = true;
				vecWorking.resize(vecNum.size() - 1);
				for (int i = 0; i < vecWorking.size(); ++i)
				{
					vecWorking[i] = vecNum[i + 1] - vecNum[i];
					if (vecWorking[i] != 0)
						bAllZero = false;
				}
				vecFirstNum.push_back(vecNum.front());
				vecLastNum.push_back(vecNum.back());

				swap(vecWorking, vecNum);
			}

			iSum1 = accumulate(vecLastNum.begin(), vecLastNum.end(), iSum1);
			int localSum = accumulate(vecFirstNum.rbegin(), vecFirstNum.rend(), 0, [](int sum, int cur) { return cur - sum; });
			iSum2 += localSum;
		}

		cout << "Part1: " << iSum1 << endl;
		cout << "Part2: " << iSum2 << endl;
	}
} // namespace Day9