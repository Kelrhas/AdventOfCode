#include "stdafx.h"

namespace Day18
{
	struct Cube
	{
		int minX = 0, maxX = 0;
		int minY = 0, maxY = 0;
		int minZ = 0, maxZ = 0;

		size_t GetSizeX() const { return maxX - minX; }
		size_t GetSizeY() const { return maxY - minY; }
		size_t GetSizeZ() const { return maxZ - minZ; }
		size_t GetVolume() const { return GetSizeX() * GetSizeY() * GetSizeZ(); }
		size_t GetArea() const { return GetSizeX() * GetSizeY() * 2 + GetSizeX() * GetSizeZ() * 2 + GetSizeY() * GetSizeZ() * 2; }

		bool Merge(const Cube &rhs)
		{
			const size_t sizeX	   = GetSizeX();
			const size_t sizeY	   = GetSizeY();
			const size_t sizeZ	   = GetSizeZ();
			const size_t rhsSizeX  = rhs.GetSizeX();
			const size_t rhsSizeY  = rhs.GetSizeY();
			const size_t rhsSizeZ  = rhs.GetSizeZ();
			const bool	 sameSizeX = sizeX == rhsSizeX;
			const bool	 sameSizeY = sizeY == rhsSizeY;
			const bool	 sameSizeZ = sizeZ == rhsSizeZ;

			if (sameSizeX)
			{
				if (sameSizeY)
				{
					if (minX == rhs.minX && minY == rhs.minY && (maxZ == rhs.minZ || minZ == rhs.maxZ)) // merge along Z axis
					{
						int newMin = glm::min(minZ, rhs.minZ);
						int newMax = glm::max(maxZ, rhs.maxZ);
						minZ	   = newMin;
						maxZ	   = newMax;
						return true;
					}
				}

				if (sameSizeZ)
				{
					if (minX == rhs.minX && minZ == rhs.minZ && (maxY == rhs.minY || minY == rhs.maxY)) // merge along Y axis
					{
						int newMin = glm::min(minY, rhs.minY);
						int newMax = glm::max(maxY, rhs.maxY);
						minY	   = newMin;
						maxY	   = newMax;
						return true;
					}
				}
			}


			if (sameSizeY && sameSizeZ)
			{
				if (minY == rhs.minY && minZ == rhs.minZ && (maxX == rhs.minX || minX == rhs.maxX)) // merge along Y axis
				{
					int newMin = glm::min(minX, rhs.minX);
					int newMax = glm::max(maxX, rhs.maxX);
					minX	   = newMin;
					maxX	   = newMax;
					return true;
				}
			}


			return false;
		}

		bool Overlaps(const Cube &rhs) const
		{
			if (minX >= rhs.maxX || maxX <= rhs.minX)
				return false;
			if (minY >= rhs.maxY || maxY <= rhs.minY)
				return false;
			if (minZ >= rhs.maxZ || maxZ <= rhs.minZ)
				return false;
			return true;
		}

		bool operator==(Cube const &rhs)
		{
			return minX == rhs.minX && minY == rhs.minY && minZ == rhs.minZ && maxX == rhs.maxX && maxY == rhs.maxY && maxZ == rhs.maxZ;
		}
	};

	enum Axis
	{
		X,
		Y,
		Z
	};
	struct Face
	{
		Axis	   normal;
		glm::ivec3 coord;
		bool	   bPositiveAxis = false;

		bool operator==(Face const &rhs) const { return normal == rhs.normal && coord == rhs.coord; }
	};
}

template<>
struct std::hash<Day18::Face>
{
	auto operator()(const Day18::Face &f) const
	{
		size_t const h1 = 1llu << f.normal;
		size_t const h2 = hash<glm::ivec3> {}(f.coord);
		return ((h1 ^ (h2 << 1)));
	}
};

namespace Day18
{
	void Solve()
	{
		cout << "--- Day 18: Boiling Boulders ---" << endl;
		vector<Cube>		cubes;
		unordered_set<Face> faces;

		ifstream is("Data18.txt");
		string	 line;

		Cube shellCube;

		while (getline(is, line))
		{
			Cube c;
			sscanf_s(line.c_str(), "%d, %d, %d", &c.minX, &c.minY, &c.minZ);
			c.maxX		   = c.minX + 1;
			c.maxY		   = c.minY + 1;
			c.maxZ		   = c.minZ + 1;
			shellCube.minX = glm::min(shellCube.minX, c.minX);
			shellCube.minY = glm::min(shellCube.minY, c.minY);
			shellCube.minZ = glm::min(shellCube.minZ, c.minZ);
			shellCube.maxX = glm::max(shellCube.maxX, c.maxX);
			shellCube.maxY = glm::max(shellCube.maxY, c.maxY);
			shellCube.maxZ = glm::max(shellCube.maxZ, c.maxZ);

			// try to merge simple cubes before pushing
			bool bMerge = false;
			for (auto &existing : cubes)
			{
				if (existing.Merge(c))
				{
					bMerge = true;
					break;
				}
			}
			if (!bMerge)
				cubes.push_back(c);


			vector<Face> pending;
			pending.push_back(Face { Axis::X, { c.minX, c.minY, c.minZ } });
			pending.push_back(Face { Axis::X, { c.maxX, c.minY, c.minZ }, true });
			pending.push_back(Face { Axis::Y, { c.minX, c.minY, c.minZ } });
			pending.push_back(Face { Axis::Y, { c.minX, c.maxY, c.minZ }, true });
			pending.push_back(Face { Axis::Z, { c.minX, c.minY, c.minZ } });
			pending.push_back(Face { Axis::Z, { c.minX, c.minY, c.maxZ }, true });


			for (auto it = pending.begin(); it != pending.end();)
			{
				auto itSet = faces.find(*it);
				if (itSet != faces.end()) // already present -> remove it
				{
					faces.erase(itSet);
					it = pending.erase(it);
				}
				else
				{
					++it;
				}
			}

			for (auto &f : pending)
			{
				faces.insert(f);
			}
		}

		// simplification step : merging complex cubes
		bool bMerge = true;
		while (bMerge)
		{
			bMerge = false;

			for (auto it = cubes.begin(); it != cubes.end(); ++it)
			{
				for (auto &rhs : cubes)
				{
					if (it->Merge(rhs))
					{
						bMerge = true;

						auto itErase = std::find(cubes.begin(), cubes.end(), rhs);
						assert(itErase != cubes.end());
						cubes.erase(itErase);

						break;
					}
				}
				if (bMerge)
					break;
			}
		}

		// size_t area = 0;
		// for (auto &c : cubes)
		//{
		//	// cout << "Cube: " << c.minX
		//	area += c.GetArea();
		//}

		cout << "Part1:" << faces.size() << endl;


		// floodfill from a face to find if it reaches the outer shell
		vector<Face> removePending;
		for (const auto &f : faces)
		{
			bool					  bOutside = false;
			unordered_set<glm::ivec3> visited;
			queue<glm::ivec3>		  visitPending;
			vector<Face>			  connectedFaces;

			// if (f.coord == glm::ivec3 { 2, 2, 5 })
			//{
			//	__debugbreak();
			//}

			auto AddVisit = [&](glm::ivec3 coord) {
				if (visited.insert(coord).second)
					visitPending.push(coord);
			};

			auto TryAddVisit = [&](glm::ivec3 coord) {
				if (visited.find(coord) != visited.end())
					return;

				Cube self;
				self.minX = coord.x;
				self.minY = coord.y;
				self.minZ = coord.z;
				self.maxX = self.minX + 1;
				self.maxY = self.minY + 1;
				self.maxZ = self.minZ + 1;

				// check if this is inside a cube
				for (const auto &c : cubes)
				{
					if (c.Overlaps(self)) // overlaps
					{
						return;
					}
				}

				visited.insert(coord);
				visitPending.push(coord);
			};

			if (f.bPositiveAxis)
				AddVisit(f.coord);
			else
			{
				switch (f.normal)
				{
					case X: AddVisit(f.coord - glm::ivec3 { 1, 0, 0 }); break;
					case Y: AddVisit(f.coord - glm::ivec3 { 0, 1, 0 }); break;
					case Z: AddVisit(f.coord - glm::ivec3 { 0, 0, 1 }); break;
				}
			}

			while (!visitPending.empty() && !bOutside)
			{
				glm::ivec3 coord = visitPending.front();
				visitPending.pop();
				if (coord.x - 1 >= shellCube.minX)
					TryAddVisit(coord - glm::ivec3 { 1, 0, 0 });
				else
					bOutside = true;
				if (coord.x + 1 < shellCube.maxX)
					TryAddVisit(coord + glm::ivec3 { 1, 0, 0 });
				else
					bOutside = true;
				if (coord.y - 1 >= shellCube.minY)
					TryAddVisit(coord - glm::ivec3 { 0, 1, 0 });
				else
					bOutside = true;
				if (coord.y + 1 < shellCube.maxY)
					TryAddVisit(coord + glm::ivec3 { 0, 1, 0 });
				else
					bOutside = true;
				if (coord.z - 1 >= shellCube.minZ)
					TryAddVisit(coord - glm::ivec3 { 0, 0, 1 });
				else
					bOutside = true;
				if (coord.z + 1 < shellCube.maxZ)
					TryAddVisit(coord + glm::ivec3 { 0, 0, 1 });
				else
					bOutside = true;
			}

			if (!bOutside)
			{
				// remove all faces linked to that volume
				removePending.push_back(f);
			}
		}

		for (const auto &f : removePending)
		{
			auto it = faces.find(f);
			assert(it != faces.end());
			faces.erase(it);
		}

		cout << "Part2:" << faces.size() << endl;
	}
} // namespace Day18