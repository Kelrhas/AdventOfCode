

void Solve()
{
	ifstream is("Data15.txt");
	string	 line;

	struct Sensor
	{
		glm::ivec2 sensor;
		glm::ivec2 beacon;
		int		   dist;
	};

	vector<Sensor> sensors;
	while (getline(is, line))
	{
		size_t off = line.find_first_of('=');
		glm::ivec2 sensor, beacon;
		sscanf_s(line .c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor.x, &sensor.y, &beacon.x, &beacon.y);

		const int dist = ManhatanDist(sensor, beacon);

		//min = glm::min(min, 
		sensors.push_back(Sensor{sensor, beacon, dist});
	}


	//int				   row = 2000000;
	//unordered_set<int> freePos;
	//for (const auto &[sensor, beacon, dist] : sensors)
	//{
	//	const int truc = dist - glm::abs(row - sensor.y);
	//	glm::ivec2		  leftPos	= {sensor.x - truc, row};
	//	glm::ivec2		  rightPos	= {sensor.x + truc, row};
	//	
	//	if (leftPos == beacon)
	//		leftPos.x++;
	//	if (rightPos == beacon)
	//		rightPos.x--;
	//	for (int i = leftPos.x; i <= rightPos.x; ++i)
	//		freePos.insert({i, row});
	//}

	//cout << "Part1: " << freePos.size() << endl;
	
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
				const auto &[sensor, beacon, beaconDist] = *it;

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
						it = tempSensors.erase(it);
						bChange = true;
					}
				}
				else //if (distressPos.x < sensor.x) // scanning from the left
				{
					const int dist = ManhatanDist(distressPos, sensor);
					if (dist <= beaconDist)
					{
						const int width	   = beaconDist - glm::abs(distressPos.y - sensor.y);
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
				if (distressPos.y % 10000 == 0)
					cout << "Y=" << distressPos.y << endl;
				break; // to get back every sensors
			}
			else if (distressPos.y >= maxPos)
			{
				break;
			}
			else if(!bChange) // no sensor have been removed, we must have the correct position
			{
				goto end;
			}
		}

	}

end:
	size_t x = distressPos.x;
	size_t y = distressPos.y;
	cout << "Part2: " << distressPos << " -> " << x * 4000000 + y << endl;

}
