#pragma once

#include <iostream>
#include <cmath>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
//template <typename T>
class N_Container {
	public:
		
		using value_type = typename std::allocator_traits<Allocator>::value_type;
		struct Node
		{
			Node* prev;
			Node* next;
			T value;

			Node() :prev{ nullptr }, next{ nullptr } {};
		};

		using NodeAllocator = typename Allocator::template rebind<Node>::other;


		N_Container() : size_container{ 0 }, first_node{ nullptr }, last_node{ nullptr } {};

		N_Container(const N_Container &other_container)
		{      
			size_container = other_container.size_container;

			Node* prev = nullptr;
			Node* current_node = nullptr;


			if (size_container>0)
			{
				first_node = node_allocator.allocate(1);
				node_allocator.construct(first_node, Node{});
				first_node->value = other_container.first_node->value;

				current_node =  other_container.first_node;
				prev = first_node;
				last_node = first_node;
			}
			


			for (int i = 1; i < size_container; i++)
			{
				current_node = current_node->next;

				Node* new_node = node_allocator.allocate(1);
				node_allocator.construct(new_node, Node{});

				new_node->value = current_node->value;
				new_node->prev = prev;

				prev->next = new_node;

				prev = new_node;
				last_node = new_node;

			}

			node_allocator = other_container.node_allocator;
			


    	}

		~N_Container() {
			for (int i = 0; i < size_container-1; i++)
			{
				first_node = first_node->next;
				node_allocator.destroy(first_node->prev);
				node_allocator.deallocate(first_node->prev, 1);
//				node_allocator.destroy(first_node->prev);
			}
//			delete first_node;
			node_allocator.destroy(first_node);
			node_allocator.deallocate(first_node, 1);
		}


	private:
		Node* current_element;
		NodeAllocator node_allocator;
	
	public:


		void push_back(T element) {
        	Node* new_element = node_allocator.allocate(1);
			node_allocator.construct(new_element, Node{});									
//			Node* new_element = new Node{};
			if (size_container == 0)
			{
				new_element->value = element;
				first_node = new_element;
				last_node = new_element;

			}
			else
			{
				last_node->next = new_element;

				new_element->prev = last_node;
				new_element->value = element;

				last_node = new_element;

			}

			size_container += 1;

		}

		void insert(int id, T value) {								
			if ((id > size_container) || (id < 0)) {
				std::cout << "Array index is out of bound";
				exit(0);
			}

			int id_element = id - 1;
			Node* position_node = first_node;

			for (int i = 0; i <= id_element; ++i)
			{
				if (i == id_element)
				{
//					Node* new_node = new Node{};
					Node* new_node = node_allocator.allocate(1);
					node_allocator.construct(new_node, Node{});
					new_node->value = value;

					new_node->prev = position_node->prev;
					new_node->next = position_node;

					position_node->prev = new_node;

					if (id_element == 0) { first_node = new_node; }
					else { new_node->prev->next = new_node; }

				}
				else
				{
					position_node = position_node->next;
				}
			}

			size_container += 1;
		}


		void erase(int id) {										
			if ((id > size_container) || (id < 0)) {
				std::cout << "Array index is out of bound";
				exit(0);
			}

			int id_element = id - 1;
			Node* position_node = first_node;

			if (id_element == 0) {
				position_node = first_node->next;
				position_node->prev = nullptr;
				node_allocator.destroy(first_node);
				node_allocator.deallocate(first_node, 1);
			
				first_node = position_node;
			}
			else if (id == size_container)
			{
				position_node = last_node->prev;
				position_node->next = nullptr;
				node_allocator.destroy(last_node);
				node_allocator.deallocate(last_node, 1);
				

				last_node = position_node;
			}
			else {
				for (int i = 0; i <= id_element; i++)
				{
					if (i == id_element)
					{
						position_node->prev->next = position_node->next;
						position_node->next->prev = position_node->prev;

						node_allocator.destroy(position_node);
						node_allocator.deallocate(position_node, 1);

					}
					else
					{
						position_node = position_node->next;
					}
				}
			}

			size_container -= 1;
		}

		void print_elements() {
			Node* st = first_node;										
			for (int i = 0; i < size_container; i++)
			{
				std::cout << st->value << " ";
				st = st->next;
			}
			std::cout << std::endl;
		}

		T& operator[](int index) {					
			if (index > size_container) {
				std::cout << "Array index is out of bound";
				exit(0);
			}
			Node* current_node = first_node;
			for (int i = 0; i != index; i++)
			{
				current_node = current_node->next;
			}
			return (current_node->value);
		}


		int size() { return size_container; }			

		friend std::ostream& operator<<(std::ostream& os, N_Container& container) {
			N_Container::Node*  current_node = container.get_first_node();

			for (int i = 0; container.size() > i; ++i) {
				os << current_node->value << " ";
				current_node = current_node->next;
			}
			return os;
		}

		N_Container& operator+=(T value) {
			push_back(value);
			return *this;
		}

		Node* get_first_node() { return first_node; }  

		Node* get_last_element() {return last_node; }

		struct Iterator
		{
			Iterator(Node* first_element) : current_element(first_element) {};

			T operator*() const { return current_element->value; }
			Iterator operator++() { current_element = current_element->next; return *this; }
			friend bool operator!=(const Iterator& first, const Iterator& second) { return first.current_element != second.current_element; }
			friend bool operator==(const Iterator& first, const Iterator& second) { return first.current_element == second.current_element; }
			
			private:
			Node* current_element;

		
		};

		Iterator begin() { return Iterator(first_node); };
		Iterator end() {return Iterator(last_node->next);}

	private:

		int size_container;						
		Node* first_node;						
		Node* last_node;						




};

