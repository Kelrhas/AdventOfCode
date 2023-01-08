#include "stdafx.h"

namespace Day6
{
	void Solve()
	{
		cout << "--- Day 6: Tuning Trouble ---" << endl;
		std::ifstream is("Data6.txt");

		string line;
		is >> line;

		auto FindEndOfPacket = [](const string &datastream, int packetLength) {
			int idx = packetLength;
			while (datastream[idx])
			{
				string_view view(datastream.c_str() + packetLength - idx, packetLength);
				bool		br = false;
				for (int i = 0; i < packetLength; ++i)
				{
					for (int j = i + 1; j < packetLength; ++j)
					{
						if (datastream[idx - i] == datastream[idx - j])
						{
							br = true;
							idx += packetLength - j;
							break;
						}
					}
					if (br)
						break;
				}

				if (!br)
				{
					return idx;
				}
			}
		};

		cout << "Part1:" << FindEndOfPacket(line, 4) + 1 << endl;
		cout << "Part2:" << FindEndOfPacket(line, 14) + 1 << endl;
	}
} // namespace Day6