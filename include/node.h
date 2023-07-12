#pragma once

//#include "searcher.h"
//#include "tuple_creator.h"
template<typename T, size_t D>
struct Node
{
	Node* prev;
	Node* next;
	T value;
	int indexes[D];

	Node() :prev{ nullptr }, next{ nullptr } {};
};









