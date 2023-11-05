#include "stdafx.h"

namespace Day5
{
	void Solve()
	{
		cout << "--- Day 5: Hydrothermal Venture ---" << endl;
		std::ifstream is("Data5.txt");

		unordered_map<glm::ivec2, int> vertAndHoriz;
		unordered_map<glm::ivec2, int> allPoints;

		string sLine, temp;
		char   c;
		int	   maxVertAndHori = 0, maxAll = 0;
		while (getline(is, sLine))
		{
			glm::ivec2 vStart, vEnd;
			stringstream ss(sLine);
			ss >> vStart.x >> c >> vStart.y >> temp >> vEnd.x >> c >> vEnd.y;

			bool vertOrHori = vStart.x == vEnd.x || vStart.y == vEnd.y;

			glm::ivec2 vAdvance(Sign(vEnd.x - vStart.x), Sign(vEnd.y - vStart.y));			
			for (glm::ivec2 vCoord = vStart; vCoord != vEnd; vCoord += vAdvance)
			{
				if (vertOrHori)
				{
					vertAndHoriz[vCoord]++;
					if (vertAndHoriz[vCoord] == 2)
						maxVertAndHori++;
				}
				allPoints[vCoord]++;
				if(allPoints[vCoord] == 2)
					maxAll++;
			}
			if (vertOrHori)
			{
				vertAndHoriz[vEnd]++;
				if (vertAndHoriz[vEnd] == 2)
					maxVertAndHori++;
			}
			allPoints[vEnd]++;
			if (allPoints[vEnd] == 2)
				maxAll++;

		}

		cout << "Part1: " << maxVertAndHori << endl;
		cout << "Part2: " << maxAll << endl;
	}
} // namespace Day5