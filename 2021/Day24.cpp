#include "stdafx.h"

namespace Day24
{
	void Solve()
	{
		cout << "--- Day 24: Arithmetic Logic Unit ---" << endl;
		struct MinMax
		{
			U64 min = 11111111111111;
			U64 max = 99999999999999;
		};
		unordered_map<U64, MinMax> cache;
		unordered_map<U64, MinMax> tmpCache;


		// z = digits[0] + 15;
		for (U64 i = 1; i <= 9; ++i)
		{
			MinMax bounds;
			bounds.min += (i - 1) * 1e13;
			bounds.max -= (9 - i) * 1e13;
			cache.insert({ 15 + i, bounds });
		}

		// z = z * 26 + (digits[1] + 12);
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + 12 + i;
				MinMax	  bounds = val;
				bounds.min += (i - 1) * 1e12;
				bounds.max -= (9 - i) * 1e12;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// z = z * 26 + (digits[2] + 15);
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + 15 + i;
				MinMax	  bounds = val;
				bounds.max -= (9 - i) * 1e11;
				bounds.min += (i - 1) * 1e11;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();


		// reduction for digit 3
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 9;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e10;
					bounds.min += (i - 1) * 1e10;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();


		// x = (z % 26) - 7;
		// if (x != digits[4])
		//	return false;
		// x = 0
		// y = 1;
		// z = (z / 26);
		// y = 0;
		// z += y;


		// reduction for digit 4
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 7;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e9;
					bounds.min += (i - 1) * 1e9;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// z = z * 26 + digits[5] + 2;
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + 2 + i;
				MinMax	  bounds = val;
				bounds.max -= (9 - i) * 1e8;
				bounds.min += (i - 1) * 1e8;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// reduction for digit 6
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 1;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e7;
					bounds.min += (i - 1) * 1e7;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// reduction for digit 7
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 16;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e6;
					bounds.min += (i - 1) * 1e6;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// z = z * 26 + digits[8] + 10;
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + 10 + i;
				MinMax	  bounds = val;
				bounds.max -= (9 - i) * 1e5;
				bounds.min += (i - 1) * 1e5;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();


		// reduction for digit 9
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 15;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e4;
					bounds.min += (i - 1) * 1e4;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// z = z * 26 + digits[10];
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + i;
				MinMax	  bounds = val;
				bounds.max -= (9 - i) * 1e3;
				bounds.min += (i - 1) * 1e3;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// z = z * 26 + digits[11];
		for (const auto &[key, val] : cache)
		{
			for (U64 i = 1; i <= 9; ++i)
			{
				const U64 newKey = key * 26 + i;
				MinMax	  bounds = val;
				bounds.max -= (9 - i) * 1e2;
				bounds.min += (i - 1) * 1e2;
				Assert(tmpCache.find(newKey) == tmpCache.end());
				Assert(cache.find(newKey) == cache.end());
				tmpCache.insert({ newKey, bounds });
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();


		// reduction for digit 12
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26) - 4;
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e1;
					bounds.min += (i - 1) * 1e1;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		// reduction for digit 13
		for (auto it = cache.begin(); it != cache.end(); ++it)
		{
			const auto &key = it->first;
			const auto &val = it->second;

			U64 x = (key % 26);
			for (U64 i = 1; i <= 9; ++i)
			{
				if (x == i)
				{
					// keep this case
					const U64 newKey = (key / 26);
					MinMax	  bounds = val;
					bounds.max -= (9 - i) * 1e0;
					bounds.min += (i - 1) * 1e0;
					Assert(cache.find(newKey) == cache.end());
					if (auto itExist = tmpCache.find(newKey); itExist != tmpCache.end())
					{
						itExist->second.min = glm::min(itExist->second.min, bounds.min);
						itExist->second.max = glm::max(itExist->second.max, bounds.max);
					}
					else
						tmpCache.insert({ newKey, bounds });
				}
			}
		}
		cache.swap(tmpCache);
		tmpCache.clear();

		Assert(cache.size() == 1);

		cout << "Part1: " << cache.begin()->second.max << endl;
		cout << "Part2: " << cache.begin()->second.min << endl;
	}
} // namespace Day24