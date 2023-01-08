

template<typename T>
struct CircleList
{
	struct Node
	{
		T	  value;
		Node *pPrev = nullptr;
		Node *pNext = nullptr;
	};

	struct iterator
	{
		Node *		   pNode = nullptr;
		CircleList<T> &ref;

		iterator(Node *_pNode, CircleList<T> &_ref) : pNode(_pNode), ref(_ref) {}
		iterator(iterator const &_it) : pNode(_it.pNode), ref(_it.ref) {}
		iterator& operator = (iterator const &) = default;

		iterator &operator++()
		{
			pNode = pNode->pNext;
			return *this;
		}
		iterator operator++(int)
		{
			iterator it = *this;
			it.pNode	= pNode;
			pNode = pNode->pNext;
			return it;
		}
		iterator &operator--()
		{
			pNode = pNode->pPrev;
			return *this;
		}
		bool operator==(iterator const &_rhs) { return pNode == _rhs.pNode; }
		bool operator!=(iterator const &_rhs) { return !(*this == _rhs); }
		T &	 operator*() { return pNode->value; }
		Node* operator->() { return pNode; }

	protected:
		iterator() = default;
	};

	iterator begin() { return iterator { m_pHead, *this }; }
	iterator end() { return iterator { m_pTail, *this }; }
	size_t	 size() const { return m_size; }

	void Push(const T &_val)
	{
		Node *pNew = new Node { _val };
		if (m_pHead == nullptr) // first add
		{
			m_pHead = pNew;
		}
		else
		{
			m_pHead->pPrev = pNew;
			pNew->pNext	   = m_pHead;
		}

		if (m_pTail)
		{
			m_pTail->pNext = pNew;
			pNew->pPrev	   = m_pTail;
		}

		m_pTail = pNew;
		m_size++;
	}

	void MoveAfter(iterator _it, iterator _where)
	{
		// remove
		_it->pPrev->pNext = _it->pNext;
		_it->pNext->pPrev = _it->pPrev;

		// add
		_where->pNext->pPrev = _it.pNode;
		_it->pNext			 = _where->pNext;
		_where->pNext		 = _it.pNode;
		_it->pPrev			 = _where.pNode;
	}

private:
	Node * m_pHead = nullptr;
	Node * m_pTail = nullptr;
	size_t m_size  = 0;
};


void Solve()
{
	ifstream is("Data20.txt");
	string	 line;

	S64				NB_INPUT = 0;
	CircleList<S64> circle;
	while (getline(is, line))
	{
		stringstream ss(line);
		int			 i;
		ss >> i;
		circle.Push(i * 811589153llu);
	}

	NB_INPUT = (int)circle.size();

	// unroll algo
	bool debug = false;


	vector<CircleList<S64>::iterator> origList;
	{
		auto it = circle.begin();
		for (size_t i = 0; i < circle.size(); ++i)
		{
			origList.push_back(it++);
		}
	}

	
	if (debug)
	{
		cout << "Init:" << endl;
		auto it = circle.begin();
		for (size_t i = 0; i < circle.size(); ++i, ++it)
			cout << *it << ", ";
		cout << endl << endl;
	}

	for (int roll = 0; roll < 10; ++roll)
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
	S64 coord1000=0;
	S64 coord2000=0;
	S64 coord3000=0;
	int idx0 = 0;

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

	cout << "Part1:" << coord1000 + coord2000 + coord3000 << endl;
}