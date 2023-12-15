#include "stdafx.h"

namespace Day15
{
	struct Label
	{
		string_view sv;
		U8			hash = 0;

		Label(string_view _sv) : sv(_sv)
		{
			for (char c : _sv)
			{
				hash = (hash + c) * 17;
			}
		}

		bool operator==(const Label &rhs) const { return hash == rhs.hash && sv.compare(rhs.sv) == 0; }
	};
} // namespace Day15

template<>
struct hash<Day15::Label>
{
	size_t operator()(const Day15::Label &label) const { return label.hash; }
};

namespace Day15
{
	void Solve()
	{
		cout << "--- Day 15: Lens Library ---" << endl;
		ifstream is("Data15.txt");

		string sLine = "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7";
		getline(is, sLine);

		list<pair<Label, int>> map[256];

		U64 uSum1 = 0, uSum2 = 0;

		for (auto view : sLine | views::split(','))
		{
			string_view sv { view.begin(), view.end() };
			// Part 1
			uSum1 += hash<Label>()(Label { sv });

			// Part 2
			size_t		uPos	= sv.find_first_of("-=");
			string_view svLabel = sv.substr(0, uPos);
			Label		lab { svLabel };
			if (uPos != sv.size() - 1) // meaning more char after
			{
				string_view svFocal = sv.substr(uPos + 1);
				int			focal	= 0;
				from_chars(svFocal.data(), svFocal.data() + svFocal.size(), focal);

				auto it = find_if(map[lab.hash].begin(), map[lab.hash].end(), [&lab](const pair<Label, int> &pair) { return pair.first == lab; });

				if (it != map[lab.hash].end())
					it->second = focal; // replacing
				else
					map[lab.hash].push_back({ lab, focal });
			}
			else
			{
				// remove from map
				auto it = find_if(map[lab.hash].begin(), map[lab.hash].end(), [&lab](const pair<Label, int> &pair) { return pair.first == lab; });

				if (it != map[lab.hash].end())
					map[lab.hash].erase(it);
			}
		}

		for (const auto &list : map)
		{
			int i = 0;
			for (const auto [lab, focal] : list)
			{
				uSum2 += (lab.hash + 1) * ++i * (focal);
			}
		}

		cout << "Part1: " << uSum1 << endl;
		cout << "Part2: " << uSum2 << endl;
	}
} // namespace Day15