#include "stdafx.h"
#include "BigInt.h"

namespace Day22
{

	struct Cube
	{
		int minX, maxX;
		int minY, maxY;
		int minZ, maxZ;

		BigInt GetSize(bool _bLimit = false) const
		{
			if (_bLimit)
			{
				if (maxX < -50 || minX > 50 || maxY < -50 || minY > 50 || maxZ < -50 || minZ > 50)
					return 0llu;

				int X = min(50, maxX) - max(-50, minX) + 1;
				int Y = min(50, maxY) - max(-50, minY) + 1;
				int Z = min(50, maxZ) - max(-50, minZ) + 1;
				return X * Y * Z;
			}
			else
			{
				BigInt X = (maxX - minX + 1);
				BigInt Y = (maxY - minY + 1);
				BigInt Z = (maxZ - minZ + 1);
				return X * Y * Z;
			}
		}
		bool Overlaps(const Cube &rhs) const
		{
			if (minX > rhs.maxX || maxX < rhs.minX)
				return false;
			if (minY > rhs.maxY || maxY < rhs.minY)
				return false;
			if (minZ > rhs.maxZ || maxZ < rhs.minZ)
				return false;
			return true;
		}

		Cube Intersect(const Cube &rhs) const
		{
			Cube c;
			c.minX = max(minX, rhs.minX);
			c.maxX = min(maxX, rhs.maxX);
			c.minY = max(minY, rhs.minY);
			c.maxY = min(maxY, rhs.maxY);
			c.minZ = max(minZ, rhs.minZ);
			c.maxZ = min(maxZ, rhs.maxZ);
			return c;
		}

		vector<Cube> Add(const Cube &rhs) const
		{
			vector<Cube> vec = rhs.Remove(Intersect(rhs));
			vec.push_back(*this);
			return vec;
		}

		vector<Cube> Remove(const Cube &rhs) const
		{
			static vector<Cube> vecSure;
			static vector<Cube> vecTest;
			static vector<Cube> vecTestTemp;
			vecSure.clear();
			vecTest.clear();
			vecTestTemp.clear();

			vecTest.push_back(*this);

			// X
			for (auto it = vecTest.begin(); it != vecTest.end(); ++it)
			{
				Cube &cTest = *it;
				if (cTest.minX < rhs.minX && cTest.maxX > rhs.maxX) // rhs entirely inside this -> keep outside and test inside
				{
					Cube c1 = cTest;
					c1.maxX = rhs.minX - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minX = rhs.maxX + 1;
					vecSure.push_back(c2);
					Cube c3 = cTest;
					c3.minX = rhs.minX;
					c3.maxX = rhs.maxX;
					vecTestTemp.push_back(c3);
				}
				else if (cTest.minX >= rhs.minX && cTest.maxX <= rhs.maxX) // this entirely inside rhs -> test whole
				{
					vecTestTemp.push_back(*this);
				}
				else if (cTest.minX < rhs.minX)
				{
					Cube c1 = cTest;
					c1.maxX = rhs.minX - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minX = rhs.minX;
					vecTestTemp.push_back(c2);
				}
				else if (cTest.maxX > rhs.maxX)
				{
					Cube c1 = cTest;
					c1.minX = rhs.maxX + 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.maxX = rhs.maxX;
					vecTestTemp.push_back(c2);
				}
			}
			vecTest.swap(vecTestTemp);
			vecTestTemp.clear();

			// Y
			for (auto it = vecTest.begin(); it != vecTest.end(); ++it)
			{
				Cube &cTest = *it;
				if (cTest.minY < rhs.minY && cTest.maxY > rhs.maxY) // rhs entirely inside this -> keep outside and test inside
				{
					Cube c1 = cTest;
					c1.maxY = rhs.minY - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minY = rhs.maxY + 1;
					vecSure.push_back(c2);
					Cube c3 = cTest;
					c3.minY = rhs.minY;
					c3.maxY = rhs.maxY;
					vecTestTemp.push_back(c3);
				}
				else if (cTest.minY >= rhs.minY && cTest.maxY <= rhs.maxY) // this entirely inside rhs -> test whole
				{
					vecTestTemp.push_back(*this);
				}
				else if (cTest.minY < rhs.minY)
				{
					Cube c1 = cTest;
					c1.maxY = rhs.minY - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minY = rhs.minY;
					vecTestTemp.push_back(c2);
				}
				else if (cTest.maxY > rhs.maxY)
				{
					Cube c1 = cTest;
					c1.minY = rhs.maxY + 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.maxY = rhs.maxY;
					vecTestTemp.push_back(c2);
				}
			}
			vecTest.swap(vecTestTemp);
			vecTestTemp.clear();


			// Z
			for (auto it = vecTest.begin(); it != vecTest.end(); ++it)
			{
				Cube &cTest = *it;
				if (cTest.minZ < rhs.minZ && cTest.maxZ > rhs.maxZ) // rhs entirely inside this -> keep outside and test inside
				{
					Cube c1 = cTest;
					c1.maxZ = rhs.minZ - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minZ = rhs.maxZ + 1;
					vecSure.push_back(c2);
					Cube c3 = cTest;
					c3.minZ = rhs.minZ;
					c3.maxZ = rhs.maxZ;
					vecTestTemp.push_back(c3);
				}
				else if (cTest.minZ >= rhs.minZ && cTest.maxZ <= rhs.maxZ) // this entirely inside rhs -> test whole
				{
					vecTestTemp.push_back(*this);
				}
				else if (cTest.minZ < rhs.minZ)
				{
					Cube c1 = cTest;
					c1.maxZ = rhs.minZ - 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.minZ = rhs.minZ;
					vecTestTemp.push_back(c2);
				}
				else if (cTest.maxZ > rhs.maxZ)
				{
					Cube c1 = cTest;
					c1.minZ = rhs.maxZ + 1;
					vecSure.push_back(c1);
					Cube c2 = cTest;
					c2.maxZ = rhs.maxZ;
					vecTestTemp.push_back(c2);
				}
			}
			vecTest.swap(vecTestTemp);
			vecTestTemp.clear();


			return vecSure;
		}
	};

	void Solve()
	{
		cout << "--- Day 22: Reactor Reboot ---" << endl;
		ifstream is("Data22.txt");

		struct CubeOp
		{
			Cube cube;
			bool set = false;
		};
		struct Operation : public CubeOp
		{
			string str;
		};
		vector<Operation> operations;

		string line;
		while (getline(is, line))
		{
			Operation op;
			op.str = line;

			const char *pStr = line.c_str() + 1;
			if (*pStr == 'n')
			{
				op.set = true;
				pStr += 4;
			}
			else
				pStr += 5;

			op.cube.minX = atoi(pStr);
			pStr		 = strstr(pStr, "..") + 2;
			op.cube.maxX = atoi(pStr);
			pStr		 = strstr(pStr, "=") + 1;
			op.cube.minY = atoi(pStr);
			pStr		 = strstr(pStr, "..") + 2;
			op.cube.maxY = atoi(pStr);
			pStr		 = strstr(pStr, "=") + 1;
			op.cube.minZ = atoi(pStr);
			pStr		 = strstr(pStr, "..") + 2;
			op.cube.maxZ = atoi(pStr);

			operations.emplace_back(move(op));
		}


		vector<CubeOp> cubes;
		vector<CubeOp> cubesTmp;
		for (const auto &op : operations)
		{
			//cout << "Doing " << op.str << endl;

			// first copy the current list
			cubesTmp.resize(0);
			cubesTmp.insert(cubesTmp.begin(), cubes.begin(), cubes.end());

			if (op.set)
			{
				// add the current operation cube
				cubesTmp.push_back({ op.cube, true });
			}

			// add the intersection with on as off
			// and the intersection with off as on
			for (const auto &c : cubes)
			{
				if (c.cube.Overlaps(op.cube))
				{
					CubeOp newOp;
					newOp.cube = c.cube.Intersect(op.cube);
					newOp.set  = !c.set;
					cubesTmp.push_back(newOp);
				}
			}

			// then swap to reiterate
			cubes.swap(cubesTmp);
		}


		BigInt sum;
		BigInt diff;
		for (auto &c : cubes)
		{
			if (c.set)
				sum += c.cube.GetSize(true);
			else
				diff += c.cube.GetSize(true);
		}

		cout << "Part1: " << (sum - diff) << endl;

		sum	 = 0llu;
		diff = 0llu;
		for (auto &c : cubes)
		{
			if (c.set)
				sum += c.cube.GetSize();
			else
				diff += c.cube.GetSize();
		}
		cout << "Part2: " << (sum - diff) << endl;
	}
} // namespace Day22