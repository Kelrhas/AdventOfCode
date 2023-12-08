#include "stdafx.h"

namespace Day8
{
	struct Node
	{
		U32 uID	   = 0;
		U32 uLeft  = 0;
		U32 uRight = 0;
	};

	U32 NodeIDFromString(string_view sv)
	{
		U32 uID = 0;
		int i = 0;
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


		array<Node, 26 * 26 * 26 * 26> nodes { 0 }; // all possible nodes from 3 letters

		string sLine;
		while (getline(is, sLine))
		{
			if (sLine.empty())
				continue;

			string_view svLine	= sLine;
			string_view svName	= svLine.substr(0, svLine.find_first_of('='));
			string_view svLeft	= svLine.substr(svLine.find_first_of('(') + 1, 3);
			string_view svRight = svLine.substr(svLine.find_first_of(',') + 1);

			U32 id = NodeIDFromString(svName);
			nodes[id].uID = id;
			nodes[id].uID = NodeIDFromString(svLeft);
			nodes[id].uID = NodeIDFromString(svRight);
		}

		Node *pCurNode = &nodes[0]; // AAA
		U32	  uEndID   = NodeIDFromString("ZZZ"sv);

		U64 uInstruction = 0;
		while (pCurNode)
		{
			char c = sInstructions[uInstruction++ % sInstructions.size()];
			if (c == 'L')
				pCurNode = &nodes[pCurNode->uLeft];
			else
				pCurNode = &nodes[pCurNode->uRight];

			if (pCurNode->uID == uEndID)
				break;
		}

		cout << "Part1: " << uInstruction << endl;
		cout << "Part2: " << endl;
	}
} // namespace Day8