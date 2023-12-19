#include "stdafx.h"

namespace Day19
{
	struct Part
	{
		union
		{
			struct
			{
				int x, m, a, s;
			};
			int arr[4];
		};
	};
	vector<Part> vecParts;

	struct PartExtent
	{
		union
		{
			struct
			{
				glm::ivec2 x, m, a, s;
			};
			glm::ivec2 arr[4] { { 1, 4000 }, { 1, 4000 }, { 1, 4000 }, { 1, 4000 } };
		};
	};

	constexpr U8 IndexFromChar(char c)
	{
		if (c == 'x')
			return 0;
		if (c == 'm')
			return 1;
		if (c == 'a')
			return 2;
		if (c == 's')
			return 3;

		Assert(false);
		return -1;
	}

	struct Rule
	{
		U8	   idxVal	 = 0;
		bool   bTestLess = false;
		S32	   iTestVal	 = false;
		string sTarget;

		bool Pass(const Part &p) const
		{
			if (idxVal == (U8)-1) // no test
				return true;

			if (bTestLess && p.arr[idxVal] < iTestVal)
				return true;
			if (!bTestLess && p.arr[idxVal] > iTestVal)
				return true;
			return false;
		}

		void ComputeExtent(PartExtent &p) const
		{
			if (idxVal != (U8)-1) // no test
			{
				if (bTestLess)
				{
					p.arr[idxVal].y = iTestVal - 1; // must be lower
				}
				else
				{
					p.arr[idxVal].x = iTestVal + 1; // must be greater
				}
			}
		}
	};

	struct Workflow
	{
		string		 sName;
		vector<Rule> rules;

		bool operator==(const Day19::Workflow &rhs) const { return false; }
	};
	unordered_map<string, Workflow> hmWorkflows;
} // namespace Day19

template<>
struct hash<Day19::Workflow>
{
	size_t operator()(const Day19::Workflow &w) const { return 1; }
};

namespace Day19
{
	void Solve()
	{
		cout << "--- Day 19: Aplenty ---" << endl;
		ifstream is("Data19.txt");


		bool bInputRules = true;

#if 1
		for (string sLine : {
			   "px{a<2006:qkq,m>2090:A,rfg}",
			   "pv{a>1716:R,A}",
			   "lnx{m>1548:A,A}",
			   "rfg{s<537:gd,x>2440:R,A}",
			   "qs{s>3448:A,lnx}",
			   "qkq{x<1416:A,crn}",
			   "crn{x>2662:A,R}",
			   "in{s<1351:px,qqz}",
			   "qqz{s>2770:qs,m<1801:hdj,R}",
			   "gd{a>3333:R,R}",
			   "hdj{m>838:A,pv}",
			   "",
			   "{x=787,m=2655,a=1222,s=2876}",
			   "{x=1679,m=44,a=2067,s=496}",
			   "{x=2036,m=264,a=79,s=2244}",
			   "{x=2461,m=1339,a=466,s=291}",
			   "{x=2127,m=1623,a=2188,s=1013}",
			 })
#else
		string sLine;
		while (getline(is, sLine))
#endif
		{
			if (sLine.empty())
			{
				bInputRules = false;
				continue;
			}

			string_view svLine = sLine;

			if (bInputRules)
			{
				string_view svName				  = svLine.substr(0, svLine.find_first_of('{'));
				hmWorkflows[(string)svName].sName = svName;

				string_view svRules = svLine.substr(svLine.find_first_of('{') + 1);
				svRules				= { svRules.begin(), svRules.end() - 1 };

				for (auto view : svRules | views::split(','))
				{
					string_view svRule { view.begin(), view.end() };
					Rule		rule;
					if (svRule.find_first_of(':') != string::npos)
					{
						rule.idxVal		  = IndexFromChar(svRule[0]);
						rule.bTestLess	  = svRule[1] == '<';
						string_view svVal = svRule.substr(2, svRule.find_first_of(':') - 2);
						from_chars(svVal.data(), svVal.data() + svVal.size(), rule.iTestVal);
						rule.sTarget = svRule.substr(svRule.find_first_of(':') + 1);
					}
					else
					{
						// only a target
						rule.idxVal	 = -1;
						rule.sTarget = svRule;
					}
					hmWorkflows[(string)svName].rules.emplace_back(rule);
				}
			}
			else
			{
				string_view svPart = svLine.substr(1, svLine.find_first_of('}') - 1);

				Part p;
				int	 idx = 0;
				for (auto view : svPart | views::split(','))
				{
					string_view sv { view.begin(), view.end() };
					string_view svVal = sv.substr(sv.find_first_of('=') + 1);
					int			val;
					from_chars(svVal.data(), svVal.data() + svVal.size(), val);
					p.arr[idx++] = val;
				}
				vecParts.emplace_back(p);
			}
		}

		bool debug = false;

		U64 uSum1 = 0;
		for (const Part &part : vecParts)
		{
			if (debug)
				cout << "Part : " << endl;
			string sCurWorkflow = "in";

			while (true)
			{
				const auto &curFlow = hmWorkflows[sCurWorkflow];

				for (const Rule &rule : curFlow.rules)
				{
					if (rule.Pass(part))
					{
						sCurWorkflow = rule.sTarget;
						if (debug)
							cout << "Using flow " << sCurWorkflow << endl;
						break;
					}
				}

				if (sCurWorkflow == "A") // accepted
				{
					uSum1 += part.x + part.m + part.a + part.s;
					break;
				}
				if (sCurWorkflow == "R") // rejected
					break;
			}
		}
		cout << "Part1: " << uSum1 << endl;

		/* Part2
			List all possible chain of acceptance, by:
			- finding the A target of all rules
			- using the PartExtent to reduce the possible values 
			- going back to all the rules targeting the current rule
			That means creating a tree of possibilities ...

			Example: the rule in "px": m>2090:A means:
			- m > 2090 by that rule
			- a >= 2006 by the previous rule being false
			- s < 1351 by the rule s<1351:px

			Maybe we should flatten the whole tree, meaning every workflow being a subtree of target
			left leaf is true, right leaf is false
			in{s<1351:px,qqz} would be

				  s<1351
				   /  \
				 px   qqz
			And replace px & qqz by their respective subtree
			px{a<2006:qkq,m>2090:A,rfg} being
					a<2006
					/	\
				  qkq   m>2090
						/   \
					   A	rfg

			And we just have to handle every leaf A and going up the tree to coùpute the extent

		*/

		vector<PartExtent> extents;
		for (const auto &[sFlowName, flow] : hmWorkflows)
		{
			// find if we have an accepted rule
			auto it = find_if(flow.rules.begin(), flow.rules.end(), [](const Rule &r) { return r.sTarget == "A"; });
			if (it != flow.rules.end())
			{
				// found one
				const Rule &r = *it;
				PartExtent	extent;
				r.ComputeExtent(extent);

				// now find every possible way to come to that rule
				// first: every previous rule must be false
				// then, find every rule leading to that workflow
			}
		}

		cout << "Part2: nb extents=" << extents.size() << endl;
	}
} // namespace Day19