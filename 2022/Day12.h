
//constexpr int GRID_HEIGHT = 5;
//constexpr int GRID_WIDTH  = 8;
constexpr int GRID_HEIGHT = 41;
constexpr int GRID_WIDTH  = 154;

struct Node
{
	char		   c;
	int			   dist = 0;
	glm::ivec2	   coord;

	bool		   bVisited = false;

	vector<Node *> neighbors;
	Node *		   pPrevious = nullptr;
};

struct CmpNodePtr
{
	bool operator()(const Node *lhs, const Node *rhs) const { return lhs->dist > rhs->dist; }
};

void Solve()
{
	Node grid[GRID_HEIGHT][GRID_WIDTH];

	ifstream is("Data12.txt");
	string	 line;
	int		 row = 0;
	Node *	 start = nullptr;
	Node *	 end   = nullptr;
	while (getline(is, line))
	{
		int col = 0;
		for (char c : line)
		{
			if (c == 'S')
			{
				start = &grid[row][col];
				c	  = 'a';
			}
			else if (c == 'E')
			{
				end = &grid[row][col];
				c	= 'z';
			}
			grid[row][col].c = c;
			grid[row][col].coord = glm::ivec2(col, row);

			col++;
		}
		++row;
	}


	for (int h = 0; h < GRID_HEIGHT; ++h)
	{
		for (int w = 0; w < GRID_WIDTH; ++w)
		{
			auto Add = [](Node &node, Node &neighbor) {
				if (neighbor.c >= node.c - 1)
					node.neighbors.push_back(&neighbor);
			};

			// manual optim
			//if constexpr (GRID_WIDTH > 10)
			//{
			//	if (grid[h][w].c == 'a' && w > 1)
			//		continue;
			//}

			if (h > 0)
				Add(grid[h][w], grid[h - 1][w]);
			if (h < GRID_HEIGHT - 1)
				Add(grid[h][w], grid[h + 1][w]);
			if (w > 0)
				Add(grid[h][w], grid[h][w - 1]);
			if (w < GRID_WIDTH - 1)
				Add(grid[h][w], grid[h][w + 1]);
		}
	}


	priority_queue<Node *, vector<Node *>, CmpNodePtr> queue;
	unordered_set<Node *>							   visited;

	queue.push(end);
	end->bVisited = true;

	while (!queue.empty())
	{
		Node *pCurrent = queue.top();
		queue.pop();

		//cout << "Looking at " << pCurrent->coord << " (" << pCurrent->dist << ")" << endl;


		for (auto &n : pCurrent->neighbors)
		{
			if (n->bVisited)
			{
				if (n != pCurrent->pPrevious && n->dist < pCurrent->dist-1)
				{
					// mutually neighbors
					auto it =find(n->neighbors.begin(), n->neighbors.end(), pCurrent);
					if (it != n->neighbors.end())
					{
						//cout << "\tReplacing " << pCurrent->pPrevious->coord << " (" << pCurrent->pPrevious->dist << ")";
						//cout << " with " << n->coord << " (" << n->dist << ")" << endl;
						pCurrent->dist		= n->dist + 1;
						pCurrent->pPrevious = n;
					}
				}
			}
			else
			{
				n->pPrevious = pCurrent;
				n->dist		 = pCurrent->dist + 1;
				n->bVisited	 = true;
				queue.push(n);

				//if (n == start) // first part
				if (n->c == 'a') // second part
				{
					cout << "Reached goal in " << n->dist << endl;
					//Node *prev = n->pPrevious;
					//while (prev)
					//{
					//	cout << '\t' << prev->coord << " -> " << prev->dist << endl;
					//	prev = prev->pPrevious;
					//}
					while (!queue.empty())
						queue.pop();
					break;
				}
			}
		}
	}


}
