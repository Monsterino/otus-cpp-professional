#pragma once
#include <vector>
#include <iostream>

#include "node.h"
/*template<typename T, T N>
void search(std::vector<std::vector<int>>& elements,std::vector<int>& compare_element, std::vector<T>& values, T value, size_t& size){
	int index = index_finder(elements, compare_element);
	
	if ((value != N) && (index == -1))
	{
		elements.push_back(compare_element);
		values.push_back(value);
		size += 1;
	}
	else if ((value != N) && (index != -1))
	{
		values[index] = value;
	}
	else if ((value == N) && (index != -1))
	{
		elements.erase(elements.begin() + index);
		values.erase(values.begin() + index);
		size -= 1;
	}
	int index_finder(const std::vector<std::vector<int>>& elements,const std::vector<int>& compare_element) {
	int index = -1;
	for (int i = 0; i < elements.size(); i++)
	{
	if (elements[i] == compare_element)
	{
	index = i;
	break;
	}
	}
	return index;
	}

	};*/

template<typename T, size_t D>
Node<T,D>* search(Node<T,D>* first_node, int* element_indexes) {
	Node<T,D>* current_node = first_node;
	int counter = 0;
	while (current_node != nullptr)
	{
		for (size_t i = 0; i < D; i++)
		{
			if (current_node->indexes[i] == *(element_indexes + i))
			{
				counter += 1;
			}
		}
		if (counter == D)
		{
			return current_node;
		}
		else
		{
			current_node = current_node->next;
		}
		counter = 0;
	}
	return nullptr;
};

template<typename T,T N, size_t D>
void element_setter(Node<T,D>*& first_node, Node<T,D>*& last_node,Node<T,D>*& searched_node,int* element_indexes, T value, size_t& size) {
	if (value != N)
	{
		if (searched_node == nullptr)
		{
			Node<T, D>* Cur_node = new Node<T, D>();
			for (size_t i = 0; i < D; i++)
			{
				Cur_node->indexes[i] = *(element_indexes + i);
			}
			Cur_node->value = value;
			if ((first_node == nullptr) && (last_node == nullptr))
			{
				first_node = Cur_node;
				last_node = Cur_node;
			}
			else
			{
				last_node->next = Cur_node;
				Cur_node->prev = last_node;
				last_node = Cur_node;
			}
			size += 1;
		}
		else
		{
			searched_node->value = value;
		}
	}

	else
	{
		if (searched_node != nullptr) {
			if (searched_node->prev!=nullptr)
			{
				searched_node->prev->next = searched_node->next;
			}
			else 
			{
				first_node = searched_node->next;
			}
			if (searched_node->next != nullptr)
			{
				searched_node->next->prev = searched_node->prev;
			}
			else
			{
				last_node = searched_node->prev;
			}
			delete searched_node;
			size -= 1;
		}
	}
}






















/*
if (first_node == nullptr)
{
	Cur_node = new Node<T, D>();
	for (size_t i = 0; i < D; i++)
	{
		Cur_node->indexes[i] = *(element_indexes + i);
	}
	Cur_node->value = value;
	return Cur_node;
}
else
{
	Cur_node = node_finder<T, N, D>(first_node, element_indexes, value)
}*/
