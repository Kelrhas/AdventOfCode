#include "stdafx.h"

namespace Day10
{
	enum Direction
	{
		NONE  = 0,
		NORTH = 1 << 0,
		EAST  = 1 << 1,
		SOUTH = 1 << 2,
		WEST  = 1 << 3
	};
	constexpr Direction Opposite(Direction dir)
	{
		Direction res = NONE;
		if (dir & Direction::NORTH)
			res = (Direction)(res | Direction::SOUTH);
		else if (dir & Direction::SOUTH)
			res = (Direction)(res | Direction::NORTH);

		if (dir & Direction::WEST)
			res = (Direction)(res | Direction::EAST);
		else if (dir & Direction::EAST)
			res = (Direction)(res | Direction::WEST);

		return res;
	}
	constexpr glm::ivec2 Offset(Direction dir)
	{
		glm::ivec2 res { 0 };
		if (dir & Direction::NORTH)
			res.y--;
		if (dir & Direction::SOUTH)
			res.y++;
		if (dir & Direction::WEST)
			res.x--;
		if (dir & Direction::EAST)
			res.x++;

		return res;
	}
	struct Node
	{
		char	   c;
		glm::ivec2 vPos;
		U32		   dist		 = -1;
		Node	  *pPrevious = nullptr;
	};
	struct CmpNodePtr
	{
		bool operator()(const Node *lhs, const Node *rhs) const { return lhs->dist > rhs->dist; }
	};

	constexpr bool IsValid(char c)
	{
		switch (c)
		{
			case '|':
			case '-':
			case 'L':
			case 'J':
			case '7':
			case 'F': return true;
		}
		return false;
	}

	constexpr Direction GetDirectionsFromChar(char c)
	{
		switch (c)
		{
			case '|': return (Direction)(Direction::NORTH | Direction::SOUTH);
			case '-': return (Direction)(Direction::WEST | Direction::EAST);
			case 'L': return (Direction)(Direction::NORTH | Direction::EAST);
			case 'J': return (Direction)(Direction::NORTH | Direction::WEST);
			case '7': return (Direction)(Direction::WEST | Direction::SOUTH);
			case 'F': return (Direction)(Direction::EAST | Direction::SOUTH);
		}
		return Direction::NONE;
	}
	constexpr char CharFromDirections(Direction dir)
	{
		if (dir == NONE)
			return '.';

		if (dir & NORTH)
		{
			if (dir & EAST)
				return 'L';
			return 'J';
		}
		if (dir & EAST)
			return 'F';
		return '7';
	}

	void Solve()
	{
		cout << "--- Day 10: Pipe Maze ---" << endl;
		ifstream is("Data10.txt");

		size_t WIDTH = 1, HEIGHT = 1;
		string sLine;
		while (getline(is, sLine))
		{
			WIDTH = max(WIDTH, sLine.size());
			HEIGHT++;
		}
		char *input = new char[WIDTH * HEIGHT] { 0 };

		int		   h = 0;
		glm::ivec2 vStart;
		is.clear();
		is.seekg(0);
		while (getline(is, sLine))
		{
			if (auto pos = sLine.find_first_of('S'); pos != string::npos)
			{
				vStart = glm::ivec2 { pos, h };
			}
			memcpy(&input[h++ * WIDTH], sLine.data(), WIDTH);
		}

		Node											   start { 'S', vStart, 0 };
		unordered_map<glm::ivec2, Node *>				   graph;
		priority_queue<Node *, vector<Node *>, CmpNodePtr> pending;

		auto GetNeighbour = [&](S32 x, S32 y, Direction dir) -> char
		{
			if (dir & Direction::NORTH)
				y--;
			if (dir & Direction::SOUTH)
				y++;
			if (dir & Direction::WEST)
				x--;
			if (dir & Direction::EAST)
				x++;

			return input[y * WIDTH + x];
		};

		// find the neighbours connected to start by looking at the neighbours
		auto HandleStart = [&](Direction dir) -> Direction
		{
			char c = GetNeighbour(vStart.x, vStart.y, dir);
			if (IsValid(c) && (GetDirectionsFromChar(c) & Opposite(dir)) != 0)
			{
				Node *pNode		 = new Node;
				pNode->c		 = c;
				pNode->dist		 = 1;
				pNode->vPos		 = vStart + Offset(dir);
				pNode->pPrevious = &start;
				pending.push(pNode);
				graph[pNode->vPos] = pNode;

				return dir;
			}

			return NONE;
		};

		Direction startDir = NONE;
		startDir		   = (Direction)(startDir | HandleStart(Direction::NORTH));
		startDir		   = (Direction)(startDir | HandleStart(Direction::SOUTH));
		startDir		   = (Direction)(startDir | HandleStart(Direction::EAST));
		startDir		   = (Direction)(startDir | HandleStart(Direction::WEST));

		// we need to update the start character for part 2
		start.c = CharFromDirections(startDir);


		graph[start.vPos] = &start;

		U32 maxDist = 0;
		while (!pending.empty())
		{
			Node *pNode = pending.top();
			pending.pop();

			Direction nodeDir = GetDirectionsFromChar(pNode->c);

			auto HandleNeighbour = [&](Direction dir)
			{
				char c = GetNeighbour(pNode->vPos.x, pNode->vPos.y, dir);
				if (IsValid(c) && (dir & nodeDir) != 0)
				{
					glm::ivec2 vNewPos = pNode->vPos + Offset(dir);
					if (graph.find(vNewPos) == graph.end())
					{
						Node *pNewNode		= new Node;
						pNewNode->c			= c;
						pNewNode->dist		= pNode->dist + 1;
						maxDist				= max(maxDist, pNewNode->dist);
						pNewNode->vPos		= vNewPos;
						pNewNode->pPrevious = pNode;
						pending.push(pNewNode);
						graph[pNewNode->vPos] = pNewNode;
					}
				}
			};

			HandleNeighbour(Direction::NORTH);
			HandleNeighbour(Direction::SOUTH);
			HandleNeighbour(Direction::EAST);
			HandleNeighbour(Direction::WEST);
		}

		cout << "Part1: " << maxDist << endl;


		// now we run though every row to count the tiles inside the loop
		// This is the same problem as in checking whether a point is in a polygon
		// https://en.wikipedia.org/wiki/Point_in_polygon
		U32		   sum = 0;
		glm::ivec2 vTestPos { 0 };
		while (vTestPos.y < HEIGHT)
		{
			int nbCrossing	= 0;
			vTestPos.x		= 0;
			Direction cumul = Direction::NONE;
			while (vTestPos.x < WIDTH)
			{
				if (auto it = graph.find(vTestPos); it != graph.end()) // in the loop
				{
					Direction			currentDir = GetDirectionsFromChar(it->second->c);
					constexpr Direction mask	   = (Direction)(NORTH | SOUTH);
					Direction			newCumul   = (Direction)(cumul ^ (currentDir & mask));
					if (cumul == Direction::NONE || newCumul == Direction::NONE)
						nbCrossing++;

					cumul = newCumul;
				}
				else
				{
					if (nbCrossing % 2 == 1)
						sum++;
				}
				vTestPos.x++;
			}
			vTestPos.y++;
		}


		cout << "Part2: " << sum << endl;
	}
} // namespace Day10