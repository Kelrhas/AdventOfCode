#include <cstdlib>

struct Obj
{
	int			  i = -1;
	vector<Obj *> list;


	/// -1 if <
	/// 0 if =
	/// 1 if >
	int Compare(Obj const &rhs)
	{
		if (i == -1 && rhs.i == -1) // both list
		{
			size_t lhsSize = list.size();
			size_t rhsSize = rhs.list.size();
			int i = 0;
			while(true)
			{
				if (lhsSize != rhsSize)
				{
					if (lhsSize == i) // no more items in left list
						return -1;
					if (rhsSize == i) // no more items in right list
						return 1;
				}
				else
				{
					if (lhsSize == i)
						return 0;
				}

				int res = list[i]->Compare(*rhs.list[i]);
				if(res != 0)
					return res;

				++i;
			}
		}
		else if (i != -1 && rhs.i != -1)
		{
			return i == rhs.i ? 0 : i < rhs.i ? -1 : 1;
		}
		else // only one is not a list
		{
			Obj tmpList, tmpInt;
			tmpList.list.push_back(&tmpInt);
			if (i == -1) // left is list
			{
				tmpInt.i = rhs.i;
				return this->Compare(tmpList);
			}
			else // right is list
			{
				tmpInt.i = i;
				return tmpList.Compare(rhs);
			}
		}

		return 0;
	}
};

template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, Obj const& o)
{
	if (o.i == -1)
	{
		out << "[";
		for (int i=0; i < o.list.size(); ++i)
		{
			Obj *c = o.list[i];
			out << *c;
			if (i < o.list.size() - 1)
				out << ",";
		}
		out << "]";
	}
	else
	{
		out << o.i;
	}

	return out;
}

char* Parse(char* str, Obj* list)
{
	// *c = '[' at first
	while (true)
	{
		char c = *str;
		if (c == '[')
		{
			Obj *newlist = new Obj;
			str			 = Parse(str + 1, newlist);
			list->list.push_back(newlist);
		}
		else if (c == ']')
		{
			return str+1;
		}
		else if (c == ',')
		{
			str++;
		}
		else
		{
			char *next;
			int	  i = strtol(str, &next, 10);
			str		= next;
			Obj *b	= new Obj;
			b->i	= i;
			list->list.push_back(b);
		}
	}
}

void Solve()
{
	ifstream is("Data13.txt");
	string	 line;
	int		 idx = 1;
	int		 sum = 0;
	vector<Obj *> packets;
	do
	{
		Obj *list = new Obj;
		getline(is, line);
		Parse(line.data()+1, list);
		packets.push_back(list);

		Obj *list2 = new Obj;
		getline(is, line);
		Parse(line.data()+1, list2);
		packets.push_back(list2);

		//cout << list << endl;
		if (list->Compare(*list2) < 0)
		{
			sum += idx;
			//cout << " < " << endl;
		}
		//else
		//	cout << " > " << endl;
		//cout << list2 << endl;
		//cout << endl;

		idx++;
	} while (getline(is, line));

	cout << "Part1:" << sum << endl;

	Obj div2;
	div2.i = 2;
	Obj div2List;
	div2List.list.push_back(&div2);
	Obj div2ListList;
	div2ListList.list.push_back(&div2List);

	Obj div6;
	div6.i = 6;
	Obj div6List;
	div6List.list.push_back(&div6);
	Obj div6ListList;
	div6ListList.list.push_back(&div6List);

	packets.push_back(&div2ListList);
	packets.push_back(&div6ListList);

	//for (Obj *o : packets)
	//	cout << *o << endl;

	std::sort(packets.begin(), packets.end(), [](Obj *lhs, Obj *rhs) { return lhs->Compare(*rhs) < 0; });

	//cout << endl;
	//for (Obj *o : packets)
	//	cout << *o << endl;

	auto it2 = std::find(packets.begin(), packets.end(), &div2ListList);
	assert(it2 != packets.end());
	int idx2 = it2 - packets.begin() + 1;

	auto it6 = std::find(packets.begin(), packets.end(), &div6ListList);
	assert(it6 != packets.end());
	int idx6 = it6 - packets.begin() + 1;

	cout << "Part2:" << idx2 * idx6 << endl;

}