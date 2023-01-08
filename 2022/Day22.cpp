#include "stdafx.h"

#include <charconv>

namespace Day22
{
	enum class Tile
	{
		EMPTY,
		GROUND,
		WALL
	};

	enum Direction
	{
		RIGHT,
		DOWN,
		LEFT,
		UP
	};

	enum class Face
	{
		NONE	= -2,
		UNKNOWN = -1,
		TOP,
		DOWN,
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		COUNT
	};

	char CharFromFace(Face f)
	{
		switch (f)
		{
			case Face::NONE: return ' ';
			case Face::UNKNOWN: return '?';
			case Face::TOP: return 'T';
			case Face::DOWN: return 'D';
			case Face::FRONT: return 'F';
			case Face::BACK: return 'B';
			case Face::LEFT: return 'L';
			case Face::RIGHT: return 'R';
		}

		return '#';
	}

	Direction TurnRight(Direction _dir) { return Direction((_dir + 1) % 4); }
	Direction TurnLeft(Direction _dir) { return Direction((_dir + 3) % 4); }


	constexpr glm::ivec2 VecFromDir(Direction dir)
	{
		switch (dir)
		{
			case RIGHT: return glm::ivec2(1, 0);
			case DOWN: return glm::ivec2(0, 1);
			case LEFT: return glm::ivec2(-1, 0);
			case UP: return glm::ivec2(0, -1);
		}
		return glm::ivec2(0, 0);
	}

	constexpr Direction DirFromVec(const glm::ivec2 &v)
	{
		if (v.x == 1 && v.y == 0)
			return RIGHT;
		else if (v.x == -1 && v.y == 0)
			return LEFT;
		else if (v.x == 0 && v.y == 1)
			return DOWN;
		else if (v.x == 0 && v.y == -1)
			return UP;

		return RIGHT;
	}

	struct Move
	{
		size_t nb		= 0;
		bool   turnLeft = false;
	};

	using Quadrant = glm::ivec2;
	struct FaceNeighbour
	{
		Quadrant to;
		int		 nbTurn = -1; // toward what quadrant and in how many right turn

		bool operator==(FaceNeighbour const &rhs) const { return to == rhs.to && nbTurn == rhs.nbTurn; }
	};
	using FaceInfo = array<FaceNeighbour, 4>;

#if 0 // example
	unordered_map<Quadrant, FaceInfo> transfoHardcode {
		{ Quadrant { 2, 0 }, FaceInfo{ FaceNeighbour{ Quadrant { 3, 2 }, 2 }, { Quadrant { 2, 1 }, 0 }, { Quadrant { 1, 1 }, 3 }, { Quadrant { 0, 1 }, 2 } } },
		{ Quadrant { 0, 1 }, FaceInfo{ FaceNeighbour{ Quadrant { 1, 1 }, 0 }, { Quadrant { 2, 2 }, 2 }, { Quadrant { 3, 2 }, 3 }, { Quadrant { 2, 0 }, 2 } } },
		{ Quadrant { 1, 1 }, FaceInfo{ FaceNeighbour{ Quadrant { 2, 1 }, 0 }, { Quadrant { 2, 2 }, 3 }, { Quadrant { 0, 1 }, 0 }, { Quadrant { 2, 0 }, 1 } } },
		{ Quadrant { 2, 1 }, FaceInfo{ FaceNeighbour{ Quadrant { 3, 2 }, 1 }, { Quadrant { 2, 2 }, 0 }, { Quadrant { 1, 1 }, 0 }, { Quadrant { 2, 0 }, 0 } } },
		{ Quadrant { 2, 2 }, FaceInfo{ FaceNeighbour{ Quadrant { 3, 2 }, 0 }, { Quadrant { 0, 1 }, 2 }, { Quadrant { 1, 1 }, 1 }, { Quadrant { 2, 1 }, 0 } } },
		{ Quadrant { 3, 2 }, FaceInfo{ FaceNeighbour{ Quadrant { 2, 0 }, 2 }, { Quadrant { 0, 1 }, 1 }, { Quadrant { 2, 2 }, 0 }, { Quadrant { 2, 1 }, 3 } } },
	};
#else
	unordered_map<Quadrant, FaceInfo> transfoHardcode {
		{ Quadrant { 1, 0 },
		  FaceInfo { FaceNeighbour { Quadrant { 2, 0 }, 0 }, { Quadrant { 1, 1 }, 0 }, { Quadrant { 0, 2 }, 2 }, { Quadrant { 0, 3 }, 1 } } },
		{ Quadrant { 2, 0 },
		  FaceInfo { FaceNeighbour { Quadrant { 1, 2 }, 2 }, { Quadrant { 1, 1 }, 1 }, { Quadrant { 1, 0 }, 0 }, { Quadrant { 0, 3 }, 0 } } },
		{ Quadrant { 1, 1 },
		  FaceInfo { FaceNeighbour { Quadrant { 2, 0 }, 3 }, { Quadrant { 1, 2 }, 0 }, { Quadrant { 0, 2 }, 3 }, { Quadrant { 1, 0 }, 0 } } },
		{ Quadrant { 0, 2 },
		  FaceInfo { FaceNeighbour { Quadrant { 1, 2 }, 0 }, { Quadrant { 0, 3 }, 0 }, { Quadrant { 1, 0 }, 2 }, { Quadrant { 1, 1 }, 1 } } },
		{ Quadrant { 1, 2 },
		  FaceInfo { FaceNeighbour { Quadrant { 2, 0 }, 2 }, { Quadrant { 0, 3 }, 1 }, { Quadrant { 0, 2 }, 0 }, { Quadrant { 1, 1 }, 0 } } },
		{ Quadrant { 0, 3 },
		  FaceInfo { FaceNeighbour { Quadrant { 1, 2 }, 3 }, { Quadrant { 2, 0 }, 0 }, { Quadrant { 1, 0 }, 3 }, { Quadrant { 0, 2 }, 0 } } },
	};
#endif

	void Solve()
	{
		cout << "--- Day 22: Monkey Map ---" << endl;
		vector<vector<Tile>> grid;

		size_t		 y = 0;
		vector<Move> moves;
		ifstream	 is("Data22.txt");
		string		 line;
		size_t		 maxLineSize = 0;
		bool		 debug		 = false;
		while (getline(is, line))
		{
			if (line.empty())
			{
				getline(is, line);

				Move		move;
				string_view sv(line);
				while (!sv.empty())
				{
					size_t leftPos	= sv.find_first_of('L');
					size_t rightPos = sv.find_first_of('R');

					size_t turnPos;
					bool   turnLeft = false;
					if (leftPos != string::npos && rightPos != string::npos)
					{
						if (leftPos < rightPos)
						{
							turnPos	 = leftPos;
							turnLeft = true;
						}
						else
						{
							turnPos	 = rightPos;
							turnLeft = false;
						}
					}
					else if (leftPos != string::npos)
					{
						turnPos	 = leftPos;
						turnLeft = true;
					}
					else if (rightPos != string::npos)
					{
						turnPos	 = rightPos;
						turnLeft = false;
					}
					else
					{
						turnPos = sv.size();
					}

					string_view number = sv.substr(0, turnPos);
					from_chars(sv.data(), sv.data() + sv.size(), move.nb);
					sv.remove_prefix(min(turnPos + 1, sv.size()));
					moves.push_back(move);
					move.turnLeft = turnLeft;
				}
			}
			else
			{
				grid.emplace_back();
				for (char c : line)
				{
					switch (c)
					{
						case ' ': grid[y].push_back(Tile::EMPTY); break;
						case '.': grid[y].push_back(Tile::GROUND); break;
						case '#': grid[y].push_back(Tile::WALL); break;
						default: assert(false);
					}
				}
				maxLineSize = max(maxLineSize, line.size());
				++y;
			}
		}

		int	   QUADRANT = gcd(maxLineSize, grid.size());
		size_t WIDTH	= maxLineSize;
		size_t HEIGHT	= grid.size();

		// fill the end of the lines
		for (auto &row : grid)
			row.resize(WIDTH, Tile::EMPTY);


		vector<vector<std::pair<Face, int>>> faces;
		faces.resize(HEIGHT / QUADRANT);
		glm::ivec2 topCoord(-1, -1);
		for (size_t h = 0; h < faces.size(); ++h)
		{
			faces[h].resize(WIDTH / QUADRANT);
			for (size_t w = 0; w < faces[h].size(); ++w)
			{
				if (grid[h * QUADRANT][w * QUADRANT] == Tile::EMPTY)
				{
					faces[h][w] = make_pair(Face::NONE, 0);
				}
				else if (topCoord.x == -1)
				{
					faces[h][w] = make_pair(Face::TOP, 0);
					topCoord.x	= w;
					topCoord.y	= h;
				}
				else
				{
					faces[h][w] = make_pair(Face::UNKNOWN, 0);
				}
			}
		}
		assert(topCoord.x != -1);


		// first thing is to find the transformation from face to face when the path goes from one quadrant to another

		const int nbFaceV = (int)faces.size();
		const int nbFaceH = (int)faces.front().size();

		// we dont need face orientation for part 1
		unordered_map<Quadrant, FaceInfo> transfoPart1;
		for (size_t h = 0; h < faces.size(); ++h)
		{
			faces[h].resize(WIDTH / QUADRANT);
			for (size_t w = 0; w < faces[h].size(); ++w)
			{
				if (faces[h][w].first != Face::NONE)
				{
					Quadrant  curQuad(w, h);
					FaceInfo &info = transfoPart1[curQuad];

					// right
					{
						FaceNeighbour neighbour;
						auto &[quad, turns] = neighbour;
						turns				= 0;
						quad				= curQuad;
						quad.x				= (quad.x + nbFaceH + 1) % nbFaceH;

						Face left = faces[quad.y][quad.x].first;
						while (left == Face::NONE)
						{
							quad.x = (quad.x + nbFaceH + 1) % nbFaceH;
							left   = faces[quad.y][quad.x].first;
						}

						info[(int)Direction::RIGHT] = neighbour;
					}
					// down
					{
						FaceNeighbour neighbour;
						auto &[quad, turns] = neighbour;
						turns				= 0;
						quad				= curQuad;
						quad.y				= (quad.y + nbFaceV + 1) % nbFaceV;

						Face left = faces[quad.y][quad.x].first;
						while (left == Face::NONE)
						{
							quad.y = (quad.y + nbFaceV + 1) % nbFaceV;
							left   = faces[quad.y][quad.x].first;
						}

						info[(int)Direction::DOWN] = neighbour;
					}
					// left
					{
						FaceNeighbour neighbour;
						auto &[quad, turns] = neighbour;
						turns				= 0;
						quad				= curQuad;
						quad.x				= (quad.x + nbFaceH - 1) % nbFaceH;

						Face left = faces[quad.y][quad.x].first;
						while (left == Face::NONE)
						{
							quad.x = (quad.x + nbFaceH - 1) % nbFaceH;
							left   = faces[quad.y][quad.x].first;
						}

						info[(int)Direction::LEFT] = neighbour;
					}
					// up
					{
						FaceNeighbour neighbour;
						auto &[quad, turns] = neighbour;
						turns				= 0;
						quad				= curQuad;
						quad.y				= (quad.y + nbFaceV - 1) % nbFaceV;

						Face left = faces[quad.y][quad.x].first;
						while (left == Face::NONE)
						{
							quad.y = (quad.y + nbFaceV - 1) % nbFaceV;
							left   = faces[quad.y][quad.x].first;
						}

						info[(int)Direction::UP] = neighbour;
					}
				}
			}
		}

		// find out how the faces are oriented to build the transformation
		unordered_map<Quadrant, FaceInfo> transfoPart2;
		{
			/* 11 ways to unfold a cube
			 * most of them 3*4 but there exist a way for 5*2
			 * we need to define a standard :
			 *
			 *		T
			 *	   LFR
			 *		D
			 *		B
			 *
			 * we keep track of the rotation of the faces in the input relative to the standard, in number of clockwise turn
			 */

			auto DrawFold = [&faces]() {
				for (size_t h = 0; h < faces.size(); ++h)
				{
					for (size_t w = 0; w < faces[h].size(); ++w)
					{
						if (faces[h][w].first == Face::NONE)
							cout << "   ";
						else
							cout << CharFromFace(faces[h][w].first) << faces[h][w].second << ' ';
					}
					cout << endl;
				}
			};


			struct Edge
			{
				Face f1;
				Face f2;
			};

			vector<Edge> edges {
				{ Face::TOP, Face::RIGHT },	  { Face::TOP, Face::FRONT },  { Face::TOP, Face::LEFT },	{ Face::TOP, Face::BACK },
				{ Face::FRONT, Face::RIGHT }, { Face::FRONT, Face::DOWN }, { Face::FRONT, Face::LEFT }, { Face::DOWN, Face::RIGHT },
				{ Face::DOWN, Face::BACK },	  { Face::DOWN, Face::LEFT },  { Face::BACK, Face::RIGHT }, { Face::BACK, Face::LEFT },
			};

			auto RemoveEdge = [&edges](Face f1, Face f2) {
				for (auto it = edges.begin(); it != edges.end(); ++it)
				{
					if (it->f1 == f1 && it->f2 == f2 || it->f1 == f2 && it->f2 == f1)
					{
						// edges.erase(it);
						return;
					}
				}
				assert(false); // trying to remove an edge already removed
			};


			int nbFound = 1;
			while (nbFound < 6)
			{
				bool bChange = false;
				for (size_t h = 0; h < nbFaceV; ++h)
				{
					for (size_t w = 0; w < nbFaceH; ++w)
					{
						auto [currentFace, currentNbTurn] = faces[h][w];
						if (currentFace == Face::NONE)
							continue;
						if (currentFace == Face::UNKNOWN)
							continue;

						auto InBounds = [nbFaceH, nbFaceV](const glm::ivec2 &v) -> bool {
							return v.x >= 0 && v.x < nbFaceH && v.y >= 0 && v.y < nbFaceV;
						};

						// retrieve the face next to the current, relative to its rotation

						const Quadrant vCurrent(w, h);
						const Quadrant vLeft  = glm::ivec2(glm::rotate(glm::vec2(-1, 0), currentNbTurn * glm::half_pi<float>())) + vCurrent;
						const Quadrant vRight = glm::ivec2(glm::rotate(glm::vec2(1, 0), currentNbTurn * glm::half_pi<float>())) + vCurrent;
						const Quadrant vUp	  = glm::ivec2(glm::rotate(glm::vec2(0, -1), currentNbTurn * glm::half_pi<float>())) + vCurrent;
						const Quadrant vDown  = glm::ivec2(glm::rotate(glm::vec2(0, 1), currentNbTurn * glm::half_pi<float>())) + vCurrent;

						if (InBounds(vLeft))
						{
							auto &[left, turns] = faces[vLeft.y][vLeft.x];
							if (left == Face::UNKNOWN)
							{
								switch (currentFace)
								{
									case Face::TOP:
										left	= Face::LEFT;
										turns	= (currentNbTurn + 1) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									case Face::FRONT:
										left	= Face::LEFT;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									case Face::DOWN:
										left	= Face::LEFT;
										turns	= (currentNbTurn + 3) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									case Face::BACK:
										left	= Face::LEFT;
										turns	= (currentNbTurn + 2) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									case Face::LEFT:
										left	= Face::BACK;
										turns	= (currentNbTurn + 2) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									case Face::RIGHT:
										left	= Face::FRONT;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, left);
										break;
									default: assert(false);
								}
							}
						}

						if (InBounds(vRight))
						{
							auto &[right, turns] = faces[vRight.y][vRight.x];
							if (right == Face::UNKNOWN)
							{
								switch (currentFace)
								{
									case Face::TOP:
										right	= Face::RIGHT;
										turns	= (currentNbTurn + 3) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									case Face::FRONT:
										right	= Face::RIGHT;
										turns	= 0;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									case Face::DOWN:
										right	= Face::RIGHT;
										turns	= (currentNbTurn + 1) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									case Face::BACK:
										right	= Face::RIGHT;
										turns	= (currentNbTurn + 2) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									case Face::RIGHT:
										right	= Face::BACK;
										turns	= (currentNbTurn + 2) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									case Face::LEFT:
										right	= Face::FRONT;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, right);
										break;
									default: assert(false);
								}
							}
						}

						if (InBounds(vUp))
						{
							auto &[up, turns] = faces[vUp.y][vUp.x];
							if (up == Face::UNKNOWN)
							{
								switch (currentFace)
								{
									case Face::TOP:
										assert(false); // we should never come here as we base everything on the first tile being the top
										up		= Face::BACK;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
									case Face::FRONT:
										assert(false); // we should never come here as we base everything on the first tile being the top
										up		= Face::TOP;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
									case Face::DOWN:
										up		= Face::FRONT;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
									case Face::BACK:
										up		= Face::DOWN;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
									case Face::LEFT:
										assert(false); // we should never come here as we base everything on the first tile being the top
										up		= Face::TOP;
										turns	= (currentNbTurn + 3) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
									case Face::RIGHT:
										assert(false); // we should never come here as we base everything on the first tile being the top
										up		= Face::TOP;
										turns	= (currentNbTurn + 1) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, up);
										break;
								}
							}
						}
						if (InBounds(vDown))
						{
							auto &[down, turns] = faces[vDown.y][vDown.x];
							if (down == Face::UNKNOWN)
							{
								switch (currentFace)
								{
									case Face::TOP:
										down	= Face::FRONT;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
									case Face::FRONT:
										down	= Face::DOWN;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
									case Face::DOWN:
										down	= Face::BACK;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
									case Face::BACK:
										assert(false); // we should never come here as we base everything on the first tile being the top
										down	= Face::TOP;
										turns	= currentNbTurn;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
									case Face::LEFT:
										down	= Face::DOWN;
										turns	= (currentNbTurn + 1) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
									case Face::RIGHT:
										down	= Face::DOWN;
										turns	= (currentNbTurn + 3) % 4;
										bChange = true;
										nbFound++;
										RemoveEdge(currentFace, down);
										break;
								}
							}
						}
					}
				}


				if (!bChange)
				{
					cout << "Only found:" << nbFound << endl;
					DrawFold();
					cout << endl;
					break;
				}
			}

			assert(nbFound == 6);
			if (debug)
				DrawFold();

			auto FindFace = [&faces](Face f) -> pair<Quadrant, int> {
				for (size_t h = 0; h < faces.size(); ++h)
				{
					for (size_t w = 0; w < faces[h].size(); ++w)
					{
						if (faces[h][w].first == f)
							return make_pair(Quadrant { w, h }, faces[h][w].second);
					}
				}
			};


			for (auto it = edges.begin(); it != edges.end();)
			{
				auto [quad1, turn1] = FindFace(it->f1);
				auto [quad2, turn2] = FindFace(it->f2);


				glm::ivec2					rotation = quad2 - quad1;
				static constexpr glm::ivec2 vLeft(-1, 0);
				static constexpr glm::ivec2 vRight(1, 0);
				static constexpr glm::ivec2 vUp(0, -1);
				static constexpr glm::ivec2 vDown(0, 1);
				int							dist = ManhatanDist(quad1, quad2);

				if (dist == 1) // faces touching
				{
					Direction dir1					 = DirFromVec(rotation);
					transfoPart2[quad1][dir1].to	 = quad2;
					transfoPart2[quad1][dir1].nbTurn = 0;
					assert(transfoPart2[quad1][dir1] == transfoHardcode[quad1][dir1]);
					Direction dir2					 = DirFromVec(-rotation);
					transfoPart2[quad2][dir2].to	 = quad1;
					transfoPart2[quad2][dir2].nbTurn = 0;
					assert(transfoPart2[quad2][dir2] == transfoHardcode[quad2][dir2]);

					it = edges.erase(it);
					continue;
				}

				if (dist == 2) // one turn
				{
					if (rotation == vLeft + vDown)
					{
						Quadrant quadLeft = quad1 + vLeft;
						Quadrant quadDown = quad1 + vDown;
						if (faces[quadLeft.y][quadLeft.x].first == Face::NONE)
						{
							assert(faces[quadDown.y][quadDown.x].first != Face::NONE);
							transfoPart2[quad1][Direction::LEFT].to		= quad2;
							transfoPart2[quad1][Direction::LEFT].nbTurn = 3;
							transfoPart2[quad2][Direction::UP].to		= quad1;
							transfoPart2[quad2][Direction::UP].nbTurn	= 1;
							assert(transfoPart2[quad1][Direction::LEFT] == transfoHardcode[quad1][Direction::LEFT]);
							assert(transfoPart2[quad2][Direction::UP] == transfoHardcode[quad2][Direction::UP]);
						}
						else
						{
							assert(faces[quadLeft.y][quadLeft.x].first != Face::NONE);
							transfoPart2[quad1][Direction::DOWN].to		 = quad2;
							transfoPart2[quad1][Direction::DOWN].nbTurn	 = 1;
							transfoPart2[quad2][Direction::RIGHT].to	 = quad1;
							transfoPart2[quad2][Direction::RIGHT].nbTurn = 3;
							assert(transfoPart2[quad1][Direction::DOWN] == transfoHardcode[quad1][Direction::DOWN]);
							assert(transfoPart2[quad2][Direction::RIGHT] == transfoHardcode[quad2][Direction::RIGHT]);
						}
						it = edges.erase(it);
						continue;
					}

					if (rotation == vRight + vUp)
					{
						Quadrant quadRight = quad1 + vRight;
						Quadrant quadUp	   = quad1 + vUp;
						if (faces[quadRight.y][quadRight.x].first == Face::NONE)
						{
							assert(faces[quadUp.y][quadUp.x].first != Face::NONE);
							transfoPart2[quad1][Direction::RIGHT].to	 = quad2;
							transfoPart2[quad1][Direction::RIGHT].nbTurn = 3;
							transfoPart2[quad2][Direction::DOWN].to		 = quad1;
							transfoPart2[quad2][Direction::DOWN].nbTurn	 = 1;
							assert(transfoPart2[quad1][Direction::RIGHT] == transfoHardcode[quad1][Direction::RIGHT]);
							assert(transfoPart2[quad2][Direction::DOWN] == transfoHardcode[quad2][Direction::DOWN]);
						}
						else
						{
							assert(faces[quadRight.y][quadRight.x].first != Face::NONE);
							transfoPart2[quad1][Direction::LEFT].to		= quad2;
							transfoPart2[quad1][Direction::LEFT].nbTurn = 1;
							transfoPart2[quad2][Direction::UP].to		= quad1;
							transfoPart2[quad2][Direction::UP].nbTurn	= 3;
							assert(transfoPart2[quad1][Direction::LEFT] == transfoHardcode[quad1][Direction::LEFT]);
							assert(transfoPart2[quad2][Direction::UP] == transfoHardcode[quad2][Direction::UP]);
						}
						it = edges.erase(it);
						continue;
					}

					if (rotation == vRight + vDown)
					{
						TODO(false);
					}
					if (rotation == vLeft + vUp)
					{
						TODO(false);
					}
				}
				
				if (dist == 3)
				{

				}
				++it;
			}

		}

		auto Display = [&]() {
			for (size_t y = 0; y < HEIGHT; ++y)
			{
				for (size_t x = 0; x < WIDTH; ++x)
				{
					Tile &tile = grid[y][x];
					cout << (tile == Tile::WALL ? '#' : tile == Tile::GROUND ? '.' : ' ');
				}
				cout << endl;
			}
		};

		if (debug)
			Display();

		auto Run = [&](const unordered_map<Quadrant, FaceInfo> &_transfo) {
			Direction  currentDir = RIGHT;
			glm::ivec2 pos(0, 0);
			while (grid[0][pos.x] == Tile::EMPTY)
			{
				++pos.x;
			}

			if (debug)
				cout << "Starting at " << pos << endl;

			auto GetNextPos = [&](glm::ivec2 pos, Direction dir) {
				glm::ivec2 vDir = VecFromDir(dir);

				const glm::ivec2 oldPos	 = pos;
				const Quadrant	 oldQuad = oldPos / QUADRANT;

				pos.x			 = int((pos.x + vDir.x + WIDTH) % WIDTH);
				pos.y			 = int((pos.y + vDir.y + HEIGHT) % HEIGHT);
				Quadrant newQuad = pos / QUADRANT;

				if (oldQuad != newQuad)
				{
					const FaceInfo info = _transfo.at(oldQuad);
					newQuad				= info[currentDir].to;
					int nbTurnRight		= info[currentDir].nbTurn;
					assert(nbTurnRight >= 0);

					glm::ivec2 posInQuad(pos.x % QUADRANT, pos.y % QUADRANT);

					auto RotateLeft = [QUADRANT](const glm::ivec2 &v) -> glm::ivec2 {
						return glm::ivec2(v.y, QUADRANT - v.x - 1);
					};
					auto RotateRight = [QUADRANT](const glm::ivec2 &v) -> glm::ivec2 {
						return glm::ivec2(QUADRANT - v.y - 1, v.x);
					};

					switch (nbTurnRight)
					{
						case 0: break;
						case 1: posInQuad = RotateRight(posInQuad); break;
						case 2: posInQuad = RotateRight(RotateRight(posInQuad)); break;
						case 3: posInQuad = RotateLeft(posInQuad); break;
					}

					pos = posInQuad + newQuad * QUADRANT;

					for (int i = 0; i < nbTurnRight; ++i)
						dir = TurnRight(dir);
				}

				Assert(grid[pos.y][pos.x] != Tile::EMPTY);


				std::pair<glm::ivec2, Direction> newPosAndDir(pos, dir);
				return newPosAndDir;
			};

			for (auto it = moves.begin(); it != moves.end(); ++it)
			{
				Move &move = *it;
				if (debug)
					cout << "Move:" << (move.turnLeft ? "left" : "right") << " and " << move.nb << endl;
				glm::ivec2 testPos = pos;
				if (it != moves.begin())
					currentDir = move.turnLeft ? TurnLeft(currentDir) : TurnRight(currentDir);
				for (size_t i = 0; i < move.nb; ++i)
				{
					auto [testPos, newDir] = GetNextPos(pos, currentDir);

					if (grid[testPos.y][testPos.x] == Tile::WALL)
						break;
					pos		   = testPos;
					currentDir = newDir;
					if (debug)
						cout << "\t" << pos << " facing " << currentDir << endl;
				}


				if (debug)
					cout << "{" << (pos.x + 1) << "," << (pos.y + 1) << "}," << endl;
			}

			if (debug)
				cout << "End:" << pos << " facing " << currentDir << " -> " << (1000 * (pos.y + 1) + 4 * (pos.x + 1) + currentDir) << endl;

			std::pair<glm::ivec2, Direction> endPosAndDir(pos, currentDir);
			return endPosAndDir;
		};


		std::pair<glm::ivec2, Direction> parsed = Run(transfoPart1);
		cout << "Part1:" << (1000 * (parsed.first.y + 1) + 4 * (parsed.first.x + 1) + parsed.second) << endl;

		//parsed = Run(transfoPart2);
		parsed = Run(transfoHardcode);
		cout << "Part2:" << (1000 * (parsed.first.y + 1) + 4 * (parsed.first.x + 1) + parsed.second) << endl;
	}
} // namespace Day22