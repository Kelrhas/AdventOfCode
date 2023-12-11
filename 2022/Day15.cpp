#include "stdafx.h"

namespace Day15
{
	struct Sensor
	{
		glm::ivec2 sensor;
		glm::ivec2 beacon;
		int		   dist;
		int		   minX;
	};

	void Solve()
	{
		cout << "--- Day 15: Beacon Exclusion Zone ---" << endl;

		ifstream is("Data15.txt");
		string	 line;

		vector<Sensor> sensors;
		while (getline(is, line))
		{
			size_t	   off = line.find_first_of('=');
			glm::ivec2 sensor, beacon;
			sscanf_s(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor.x, &sensor.y, &beacon.x, &beacon.y);

			const int dist = ManhattanDist(sensor, beacon);

			// min = glm::min(min,
			sensors.push_back(Sensor { sensor, beacon, dist, sensor.x - dist });
		}

		// sort by leftmost reach to accelerate scanning
		sort(sensors.begin(), sensors.end(), [](Sensor const &lhs, Sensor const &rhs) { return lhs.minX < rhs.minX; });

		int	   row	= 2000000;
		size_t sum	= 0;
		int	   maxX = sensors.front().minX;
		for (const auto &[sensor, beacon, dist, minX] : sensors)
		{
			const int truc = dist - glm::abs(row - sensor.y);
			if (truc > 0)
			{
				glm::ivec2 leftPos	= { sensor.x - truc, row };
				glm::ivec2 rightPos = { sensor.x + truc, row };

				if (leftPos == beacon)
					leftPos.x++;
				if (rightPos == beacon)
					rightPos.x--;
				if (rightPos.x <= maxX)
					continue;

				leftPos = max(leftPos, maxX + 1);

				sum += rightPos.x - leftPos.x + 1;
				maxX = max(maxX, rightPos.x);
			}
		}
		cout << "Part1: " << sum << endl;


		// check is a pos is in range of any sensor, if not it's our answer
		auto TestPos = [&](int x, int y) -> bool {
			if (x >= 0 && x <= 4'000'000 && y >= 0 && y <= 4'000'000)
			{
				glm::ivec2 pos(x, y);
				bool	   atRange = false;
				for (const auto &[sensor, beacon, dist, minX] : sensors)
				{
					if (ManhattanDist(pos, sensor) <= dist)
					{
						atRange = true;
						break;
					}
				}
				if (!atRange)
				{
					cout << "Part2: " << x * 4000000llu + y << endl;
					return true;
				}
			}

			return false;
		};

#ifdef _DEBUG
		cout << "Please run in Release, it's awfully slow in Debug due to glm (though it is actually running)" << endl;
#endif

		// test every positions around each sensor
		for (const auto &[sensor, beacon, dist, minX] : sensors)
		{
			// cout << "Sensor " << sensor << endl;
			for (int i = 1; i < dist; ++i)
			// for_each(std::execution::par_unseq, 1, dist, [sensor = sensor, dist=dist, &TestPos](int i)
			{
				if (TestPos(sensor.x - (dist - i + 1), sensor.y - i))
					return;
				if (TestPos(sensor.x + (dist - i + 1), sensor.y - i))
					return;
				if (TestPos(sensor.x - (dist - i + 1), sensor.y + i))
					return;
				if (TestPos(sensor.x + (dist - i + 1), sensor.y + i))
					return;
			}
			//);
			// and all corners
			if (TestPos(sensor.x - (dist + 1), sensor.y))
				return;
			if (TestPos(sensor.x + (dist + 1), sensor.y))
				return;
			if (TestPos(sensor.x, sensor.y - (dist + 1)))
				return;
			if (TestPos(sensor.x, sensor.y + (dist + 1)))
				return;
		}


#ifdef BRUTEFORCE

		const int  maxPos = 4000000;
		glm::ivec2 distressPos(0, 0);

		vector<Sensor> tempSensors;
		tempSensors.reserve(sensors.size());
		bool debug = false;
		while (true)
		{
			tempSensors = sensors;
			if (debug)
				cout << "#######################################################" << endl;
			while (!tempSensors.empty())
			{
				bool bChange = false;

				if (debug)
					cout << "Testing " << distressPos << endl;
				for (auto it = tempSensors.begin(); it != tempSensors.end(); ++it)
				{
					const auto &[sensor, beacon, beaconDist, leftReach] = *it;

					if (distressPos.x == sensor.x) // same col
					{
						// advance by the remaining dist
						const int remaining = beaconDist - glm::abs(sensor.y - distressPos.y);
						if (remaining > 0)
						{
							distressPos.x += remaining + 1;
							if (debug)
							{
								cout << "Hit " << sensor << " with dist=" << beaconDist << endl;
								cout << "\t ->" << distressPos << endl;
							}
							it		= tempSensors.erase(it);
							bChange = true;
						}
					}
					else // if (distressPos.x < sensor.x) // scanning from the left
					{
						const int dist = ManhatanDist(distressPos, sensor);
						if (dist <= beaconDist)
						{
							const int width = beaconDist - glm::abs(distressPos.y - sensor.y);
							distressPos.x += width + (sensor.x - distressPos.x) + 1;
							if (debug)
							{
								cout << "Hit " << sensor << " with dist=" << beaconDist << endl;
								cout << "\t ->" << distressPos << endl;
							}
							it		= tempSensors.erase(it);
							bChange = true;
						}
					}

					if (it == tempSensors.end())
						break;
				}

				if (distressPos.x >= maxPos)
				{
					// might have some clever things to do to minimize
					distressPos.x = 0;
					distressPos.y++;
					if (debug && distressPos.y % 10000 == 0)
						cout << "Y=" << distressPos.y << endl;
					break; // to get back every sensors
				}
				else if (distressPos.y >= maxPos)
				{
					break;
				}
				else if (!bChange) // no sensor have been removed, we must have the correct position
				{
					goto end;
				}
			}
		}

	end:
		size_t x = distressPos.x;
		size_t y = distressPos.y;
		cout << "Part2: " << distressPos << " -> " << x * 4000000 + y << endl;
#endif
	}
} // namespace Day15