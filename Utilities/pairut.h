/*
* Name: Pair Utilities
* Author: Robert Ciborowski
* Date: 28/01/2019
* Description: A file used for utilities regarding std::pair.
*/

#pragma once
#include <utility>

// The following structs are for when you want to create a HashMap with std::pairs as the keys.
struct PairHashMapFunctions {
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};