#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;

template<class T>
struct vector_unordered_hash
{
	std::hash<T> hasher;
	auto operator() (std::vector<T> const& key) const
	{
		size_t result = 0;
		for (const auto& element : key)
			result = result * 31 + hasher(element);
		return result;
	}
};
//
//template<template<class ...>class element_hash = std::hash>
//struct vector_unordered_hash
//{
//	std::hash<T> hasher;
//	template<class T>
//	size_t operator() (T const& t) const {
//		size_t result = element_hash<int>{}(0);
//		for (auto&& element : t)
//		{
//			using element_t = decay_t<decltype(element)>;
//			auto next = element_hash<element_t>{}(x);
//			result = result * 31 + next;
//		return result;
//	}
//};