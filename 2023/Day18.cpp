#include "stdafx.h"

namespace Day18
{
	void Solve()
	{
		cout << "--- Day 18:  ---" << endl;
		ifstream is("Data18.txt");


		vector<glm::ivec2> polygon;
		glm::ivec2		   vPos { 0 };
		glm::ivec2		   vMax = vPos;
#if 1
		for (string sLine : {
			   //"R 6 (#70c710)",
			   //"D 5 (#0dc571)",
			   //"L 2 (#5713f0)",
			   //"D 2 (#d2c081)",
			   //"R 2 (#59c680)",
			   //"D 2 (#411b91)",
			   //"L 5 (#8ceee2)",
			   //"U 2 (#caa173)",
			   //"L 1 (#1b58a2)",
			   //"U 2 (#caa171)",
			   //"R 2 (#7807d2)",
			   //"U 3 (#a77fa3)",
			   //"L 2 (#015232)",
			   //"U 2 (#7a21e3)",
				   
			   //"R 6 (#70c710)",
			   //"D 5 (#0dc571)",
			   //"L 2 (#5713f0)",
			   //"D 2 (#d2c081)",
			   //"R 2 (#59c680)",
			   //"D 2 (#411b91)",
			   //"L 6 (#8ceee2)",
			   //"U 9 (#caa173)",

			   "R 6 (#70c710)",
			   "D 9 (#0dc571)",
			   "L 6 (#8ceee2)",
			   "U 9 (#caa173)",
			 })
#else
		string sLine;
		while (getline(is, sLine))
#endif
		{
			string_view svLine	= sLine;
			string_view svDist	= svLine.substr(2, svLine.find_first_of(' ', 2) - 2);
			string_view svColor = svLine.substr(svLine.find_first_of('('));

			int dist = 1;
			from_chars(svDist.data(), svDist.data() + svDist.size(), dist);

			polygon.emplace_back(vPos);
			vMax = glm::max(vMax, vPos);

			if (sLine[0] == 'L')
				vPos.x -= dist;
			if (sLine[0] == 'R')
				vPos.x += dist;
			if (sLine[0] == 'U')
				vPos.y -= dist;
			if (sLine[0] == 'D')
				vPos.y += dist;
		}

		auto Area = [&polygon]()
		{
			S32 iArea = 0;
			// First vertex is 0,0 so not contributing
			for (int i = 0; i < polygon.size() - 1; ++i)
			{
				iArea += polygon[i].x * polygon[i + 1].y;
				iArea -= polygon[i].y * polygon[i + 1].x;
			}
			iArea += polygon[polygon.size() - 1].x * polygon[0].y;
			iArea -= polygon[polygon.size() - 1].y * polygon[0].x;

			return glm::abs(iArea) / 2;
		};


		auto Perimeter = [&polygon]()
		{
			S32 iPerim = 0;
			// First vertex is 0,0 so not contributing
			for (int i = 0; i < polygon.size() - 1; ++i)
			{
				iPerim += ManhattanDist(polygon[i], polygon[i + 1]);
			}
			iPerim += ManhattanDist(polygon[polygon.size() - 1], polygon[0]);
			return iPerim;
		};
		//


		cout << "Part1: " << Area() << " " << Perimeter() << endl;
		// 52083 too low
		cout << "Part2: " << endl;
	}
} // namespace Day18