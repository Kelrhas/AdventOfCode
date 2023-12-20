#include "BigInt.h"
#include "stdafx.h"

namespace Day20
{
	struct Node;
	struct Signal
	{
		Node *pSource = nullptr;
		Node *pNode	  = nullptr; // target
		bool  bPulse;			 // high or low
	};
	struct Node
	{
		Node(string_view sv) : sName(sv) {}

		string		   sName;
		bool		   bLastSignal = false;
		vector<Node *> targets;
		vector<string> targetNames;

		virtual bool GetSignal() const { return bLastSignal; }
		virtual U32	 OnSignal(bool bSignal, vector<Signal> &signals) = 0;
	};
	struct NodeBroadcast : public Node
	{
		NodeBroadcast(string_view sv) : Node(sv) {}

		virtual U32 OnSignal(bool bSignal, vector<Signal> &signals) override
		{
			bLastSignal = bSignal;
			for (Node *pNode : targets)
			{
				signals.emplace_back(this, pNode, bSignal);
			}
			return targets.size();
		}
	};

	struct NodeFlip : public Node
	{
		NodeFlip(string_view sv) : Node(sv) {}

		bool		bState = false;
		virtual U32 OnSignal(bool bSignal, vector<Signal> &signals) override
		{
			if (!bSignal)
			{
				bState		= !bState;
				bLastSignal = bState;

				for (Node *pNode : targets)
				{
					signals.emplace_back(this, pNode, bState);
				}
				return targets.size();
			}
			return 0;
		}
	};

	struct NodeNand : public Node
	{
		NodeNand(string_view sv) : Node(sv) {}

		vector<const Node *> sources;

		virtual U32 OnSignal(bool bSignal, vector<Signal> &signals) override
		{
			bool bAllHigh = true;
			for (const Node *pSrc : sources)
			{
				if (!pSrc->GetSignal())
				{
					bAllHigh = false;
					break;
				}
			}

			for (Node *pNode : targets)
			{
				signals.emplace_back(this, pNode, !bAllHigh);
			}
			return targets.size();
		}
	};

	unordered_map<string, Node *> nodes;

	void Solve()
	{
		cout << "--- Day 20: Propagation ---" << endl;


		vector<NodeNand *> needSources;
#if 0
		for (string sLine : {
			   //"broadcaster -> a, b, c",
			   //"%a -> b",
			   //"%b -> c",
			   //"%c -> inv",
			   //"&inv -> a",

			   "broadcaster -> a",
			   "%a -> inv, con",
			   "&inv -> b",
			   "%b -> con",
			   "&con -> output",
			 })
#else
		ifstream is("Data20.txt");
		string	 sLine;
		while (getline(is, sLine))
#endif
		{
			string_view svLine	  = sLine;
			string_view svName	  = svLine.substr(svLine.find_first_not_of("%&"));
			svName				  = svName.substr(0, svName.find_first_of(' '));
			string_view svTargets = svLine.substr(svLine.find_first_of('>') + 1);
			Node *		pCurNode  = nullptr;
			if (svLine[0] == '%')
			{
				pCurNode = new NodeFlip { svName };
			}
			else if (svLine[0] == '&')
			{
				pCurNode = new NodeNand { svName };
				needSources.emplace_back((NodeNand *)pCurNode);
			}
			else
			{
				pCurNode = new NodeBroadcast { svName };
			}

			for (auto view : svTargets | views::split(','))
			{
				string_view sv { view.begin(), view.end() };
				string_view svTarget = sv.substr(sv.find_first_not_of(' '));
				svTarget			 = svTarget.substr(0, svTarget.find_first_of(' '));

				pCurNode->targetNames.emplace_back(svTarget);
			}

			nodes[(string)svName] = pCurNode;
		}


		// find targets nodes from names
		for (const auto &[sName, pNode] : nodes)
		{
			for (string &target : pNode->targetNames)
			{
				auto  it	  = nodes.find(target);
				Node *pTarget = nullptr;
				if (it != nodes.end())
					pTarget = it->second;
				else
				{
					// it was not in the input nodes, must be a useless node but still a bit useful
					pTarget		  = new NodeBroadcast { target };
					nodes[target] = pTarget;
				}
				pNode->targets.emplace_back(pTarget);
			}
		}

		// find sources nodes from names
		for (NodeNand *pNodeNand : needSources)
		{
			for (const auto &[sName, pNode] : nodes)
			{
				for (const Node *pTarget : pNode->targets)
				{
					if (pTarget == pNodeNand)
					{
						pNodeNand->sources.emplace_back(pNode);
						break;
					}
				}
			}
		}


		bool		   debug = false;
		vector<Signal> signals;
		signals.reserve(1 << 16);

		Node *pFirst = nodes["broadcaster"];

		// push button
		BigInt sumTrue	= 0llu;
		BigInt sumFalse = 0llu;
		for (int iStep = 0; iStep < (debug ? 4 : 1000); iStep++)
		{
			signals.clear();

			size_t iStart = 0;
			size_t iEnd	  = pFirst->OnSignal(false, signals);
			while (true)
			{
				U32 uAdded = 0;
				for (size_t i = iStart; i < iEnd; i++)
				{
					uAdded += signals[i].pNode->OnSignal(signals[i].bPulse, signals);
				}

				if (uAdded == 0)
					break;
				iStart = iEnd;
				iEnd += uAdded;
			}

			if (debug)
			{
				for (const auto &sig : signals)
				{
					if (sig.bPulse)
						cout << sig.pSource->sName << " -high-> " << sig.pNode->sName << endl;
					else
						cout << sig.pSource->sName << " -low-> " << sig.pNode->sName << endl;
				}
				cout << endl;
			}

			size_t uTruePulses = count_if(signals.begin(), signals.end(), [](const Signal &sig) { return sig.bPulse; });
			sumTrue += uTruePulses;
			sumFalse += (signals.size() - uTruePulses + 1);
		}

		cout << "Part1: " << sumTrue * sumFalse << endl;
		// 680204866 too low
		cout << "Part2: " << endl;
	}
} // namespace Day20