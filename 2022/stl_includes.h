#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <execution>
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

template<class T, size_t S>
struct array_unordered_hash
{
	std::hash<T> hasher;
	auto operator() (std::array<T, S> const& key) const
	{
		size_t result = 0;
		for (const auto& element : key)
			result = result * 31 + hasher(element);
		return result;
	}
};


struct pair_hash
{
	template<class T1, class T2>
	std::size_t operator()(std::pair<T1, T2> const &pair) const
	{
		std::size_t h1 = std::hash<T1>()(pair.first);
		std::size_t h2 = std::hash<T2>()(pair.second);

		return h1 ^ h2;
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