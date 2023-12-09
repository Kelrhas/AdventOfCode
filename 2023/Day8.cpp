#include "stdafx.h"

namespace Day8
{
	struct Node
	{
		U32	   uID = -1;
		string str;
		U32	   uLeft	   = 0;
		U32	   uRight	   = 0;
		bool   bEndingNode = false;
	};

	U32 NodeIDFromString(string_view sv)
	{
		U32 uID = 0;
		int i	= 0;
		while (i < sv.size())
		{
			const char c = sv[i++];
			if (c >= 'A' && c <= 'Z')
			{
				uID *= 26;
				uID += c - 'A';
			}
		}
		return uID;
	}

	void Solve()
	{
		cout << "--- Day 8: Haunted Wasteland ---" << endl;
		ifstream is("Data8.txt");

		string sInstructions;
		getline(is, sInstructions);


		Node		  *nodes = new Node[26 * 26 * 26 * 26]; // all possible nodes from 3 letters
		vector<Node *> vecStartingNodes;

		string sLine;
		while (getline(is, sLine))
		{
			if (sLine.empty())
				continue;

			string_view svLine	= sLine;
			string_view svName	= svLine.substr(0, svLine.find_first_of('='));
			string_view svLeft	= svLine.substr(svLine.find_first_of('(') + 1, 3);
			string_view svRight = svLine.substr(svLine.find_first_of(',') + 1);

			U32 id				  = NodeIDFromString(svName);
			nodes[id].uID		  = id;
			nodes[id].str		  = svName;
			nodes[id].uLeft		  = NodeIDFromString(svLeft);
			nodes[id].uRight	  = NodeIDFromString(svRight);
			nodes[id].bEndingNode = (id % 26) == 25;

			if ((id % 26) == 0)
				vecStartingNodes.emplace_back(&nodes[id]);
		}

		Node *pCurNode = &nodes[0]; // AAA
		U32	  uEndID   = NodeIDFromString("ZZZ"sv);

		U64 uInstruction = 0;
		if (pCurNode->uID != -1)
		{
			while (pCurNode)
			{
				Assert(pCurNode->uID != -1);
				char c = sInstructions[uInstruction++ % sInstructions.size()];
				if (c == 'L')
					pCurNode = &nodes[pCurNode->uLeft];
				else
					pCurNode = &nodes[pCurNode->uRight];

				if (pCurNode->uID == uEndID)
					break;
			}
			cout << "Part1: " << uInstruction << endl;
		}

		uInstruction = 0;

		struct CycleData
		{
			U32 uNode = -1;
			U64 uFirstEnd = 0;
			U64 uSecondEnd = 0;
		};
		vector<CycleData> vecMatch(vecStartingNodes.size());
		while (true)
		{
			char c			= sInstructions[uInstruction++ % sInstructions.size()];
			int	 iNbEndNode = 0;
			for (int i = 0; i < vecStartingNodes.size(); ++i)
			{
				auto &pNode = vecStartingNodes[i];

				if (c == 'L')
					pNode = &nodes[pNode->uLeft];
				else
					pNode = &nodes[pNode->uRight];

				if (pNode->bEndingNode)
				{
					iNbEndNode++;
					if (vecMatch[i].uSecondEnd == 0) // no cycle found
					{
						if (vecMatch[i].uNode == pNode->uID) // already found that node previously
						{
							vecMatch[i].uSecondEnd = uInstruction;
						}
						else
						{
							Assert(vecMatch[i].uNode == -1);
							vecMatch[i].uNode = pNode->uID;
							vecMatch[i].uFirstEnd = uInstruction;
						}
					}
				}
			}

			if (iNbEndNode == vecStartingNodes.size())
				break;

			if (find_if(vecMatch.begin(), vecMatch.end(), [](const CycleData &data) { return data.uSecondEnd == 0; }) == vecMatch.end())
			{
				break;
			}
		}

		// found cycles, get the lcm
		vector<U64> vecCyclesLength;
		for (const CycleData &data : vecMatch)
		{
			vecCyclesLength.emplace_back(data.uSecondEnd - data.uFirstEnd);
		}

		U64 uLCM = 1;
		for (U64 u : vecCyclesLength)
		{
			uLCM = lcm(uLCM, u);
		}

		cout << "Part2: " << uLCM << endl;

		delete[] nodes;
	}
} // namespace Day8