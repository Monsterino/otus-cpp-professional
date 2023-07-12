#pragma once
#include <vector>
#include <iostream>
#include <type_traits>


#include "searcher.h"
#include "tuple_creator.h"
#include "node.h"
template<typename T, T N, size_t D =2>
class Matrix
{
public:
	Matrix() : size_m(0),first_node(nullptr),last_node(nullptr) {};
	~Matrix()
	{
		for (size_t i = 0; i < size_m - 1; i++)
		{
			first_node = first_node->next;
			delete first_node->prev;
		}
		if (first_node != nullptr) {
			delete first_node;
		}
		
	}

	Matrix(const Matrix& other) {
/*		if (first_node!= nullptr)
		{
			for (int i = 0; i < size_m - 1; i++)
			{
				first_node = first_node->next;
				delete first_node->prev;
			}
			if (first_node != nullptr) {
				delete first_node;
			}
		}*/

		if (other.first_node!=nullptr)
		{
			Node<T, D>* node = new Node<T, D>;
			std::copy(other.first_node->indexes, other.first_node->indexes + D, node->indexes);
	//		node->indexes = other.first_node->indexes;
			node->value = other.first_node->value;
			first_node = node;

			Node<T, D>* node_prev = node;
			Node<T, D>* node_nx = other.first_node->next;
			while(node_nx !=nullptr)
			{
				node = new Node<T, D>;
				std::copy(node_nx->indexes, node_nx->indexes + D, node->indexes);
//				node->indexes = node_nx->indexes;
				node->value = node_nx->value;
				node_prev->next = node;
				node->prev = node_prev;
				last_node = node;

				node_prev = node;
				node_nx = node_nx->next;
			}

		}
		else
		{
			first_node = nullptr;
			last_node = nullptr;
		}
		

	}
	
//	template<typename T, T N, size_t D>

	class virtual_element
	{
	public:

		virtual_element(Matrix<T, N, D>& main_matrix, int first_index) :main_matrix(main_matrix),num_of_indexes(1)
		{
			indexes[0] = first_index;
		}
//		virtual_element(const virtual_element&) = delete;

		virtual_element operator[](int index) {
			indexes[num_of_indexes] = index;
			num_of_indexes += 1;
			return *this;
		}

		virtual_element operator=(const T& other) {
			matrix_value = other;
			if (num_of_indexes!=(D))
			{
				throw std::out_of_range(std::to_string(D));
			}
			main_matrix.set_parameters(*this);
			return *this;
		}

		bool operator==(const T& other) {
			T element = main_matrix.get_parameters(indexes);		
			return element == other;
		}

		operator T() {
			return main_matrix.get_parameters(indexes);
		}

		
		int* get_indexes() {
			return indexes;
		}
		T get_value() {
			return matrix_value;
		}

		
	private:
		Matrix<T, N, D>& main_matrix;
		int indexes[D];
		int num_of_indexes;
		T matrix_value;
		
	};

	virtual_element operator[](int index)
	{
		return virtual_element(*this, index);
	}
	
	void set_parameters(virtual_element& element) {
		int * element_indexes = element.get_indexes();
		T value = element.get_value();
		
		Node<T,D>* searched_node = search<T,D>(first_node, element_indexes);
		element_setter<T, N, D>(first_node, last_node, searched_node, element_indexes, value,size_m);
	}

	T get_parameters(int* element_indexes) {
		Node<T,D>* searched_node = search<T,D>(first_node, element_indexes);
		return (searched_node != nullptr ? searched_node->value : N);
	}
	size_t size() {
		return size_m;
	}

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = typename tuple_typer<T,D,int>::tuple_type;
	
	Iterator(Node<T, D>* node) :current_element(node) {};
	
	value_type operator*() const
	{
		return return_tuple<T,D>(current_element);
	}
	Iterator& operator++() 
	{
		current_element= current_element->next;
		return *this;
	}

	Iterator& operator++(int)
	{
		Iterator tmp = *this;
		++(*this);
		return tmp;
	}

	friend bool operator== (const Iterator& a, const Iterator& b)
	{
		return a.current_element == b.current_element;
	};

	friend bool operator!= (const Iterator& a, const Iterator& b)
	{
		return a.current_element != b.current_element;
	};
	private:
		Node<T, D>* current_element;
	

	};
	Iterator begin() { return Iterator(first_node); };
	Iterator end() { return Iterator(last_node->next); }
	
private:
	size_t size_m;
	Node<T,D>* first_node;
	Node<T,D>* last_node;

};










