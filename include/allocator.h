#pragma once

#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>



template <class T>
class CustomAllocator {
public:
    //typedef T value_type;
	using value_type = T;
	using pointer = T*;

	struct Memory_Block {
		unsigned char* start_element;
		unsigned char* last_element;
		unsigned char* current_empty_element;

	};

    CustomAllocator () noexcept { }
    ~CustomAllocator() {
		//std::cout << "Destructor" << std::endl;
		for (auto it  = allocated_memory_blocks.begin(); it != allocated_memory_blocks.end(); ++it)
		{		
			std::free(it->start_element);
		}
	}

    template <class U> CustomAllocator  (const CustomAllocator <U>&) noexcept {}

		template <class Up, class... Args>
    	void construct(Up* p, Args&&... args) {
        	::new ((void*)p) Up(std::forward<Args>(args)...);
    	}

    	void destroy(pointer p) {
        	p->~T();
    	}
    T* allocate (std::size_t n) {
		if (n > 256) {
			throw std::bad_alloc();
		}


		//std::cout << "asdf" << std::endl;
		unsigned char* current_empty_element = nullptr;
		unsigned char* begin_of_block = nullptr;
        size_t num_elements = pow(n);
		
        size_t used_memory = num_elements * sizeof(T); //Размер, который просит выделить контейнер

        
        //Проверяем, есть ли выделенная память, или хватает ли оставшейся памяти для выдачи ее нашему контейнеру
        //Если свободных блоков памяти нет, то просто выделяем память размером - запрашиваемое + доводим до ближайшего числа - степени двойки

		for (size_t i = 0; i < allocated_memory_blocks.size(); i++)				// Проверка, есть ли хоть один блок с занятой памятью
		{
			auto block = allocated_memory_blocks[i];
			if (block.current_empty_element != nullptr)
			{
				current_empty_element = block.current_empty_element;
				break;
			}
			
		}
		
// 		std::find_if(allocated_memory_blocks.begin(), allocated_memory_blocks.end(), [&current_empty_element](const Memory_Block& block)-> void {if (block.current_empty_element != nullptr){
//																																			 current_empty_element = block.current_empty_element;
//																																		     };});

		//typename std::vector<Memory_Block>::iterator itr = std::find_if(allocated_memory_blocks.begin(), allocated_memory_blocks.end(), [&current_empty_element](const Memory_Block& block){}});



        if (current_empty_element == nullptr)
        {
			//std::cout << "Not enought memory. Size: " << n << "; Give: " << num_elements << "; Ost: " << num_elements - n << ";" << std::endl;

			begin_of_block = reinterpret_cast<unsigned char*>(malloc(used_memory)); //Выделяем память, на начало нашей памяти смотрит указалель элементов
            memory_allocate(begin_of_block,num_elements); // Разбиваем всю выделеную память на элементы, каждый элемент хранит в первом байте сдвиг на следующий свободный элемент
		
			current_empty_element = check_free_space(begin_of_block, n);

			Memory_Block block{ begin_of_block ,(begin_of_block+ used_memory),current_empty_element };
			allocated_memory_blocks.push_back(block);
			return reinterpret_cast<T*>(begin_of_block);

        }
 
		else
		{
			//std::cout << "We have free memory, check for all blocks memory needed size" << std::endl;

			//Если свободные блоки есть - проверяем, последовательно ли находятся данные блоки и хватит ли нам этой памяти
			std::pair<int, unsigned char*> min_value(0, nullptr); //Число блоков подряд, указатель на начальный блок
			int num_of_block = -1;
			// Первый цикл проходит по всем блокам и ищет участок свободной памяти, который наиболее подходит по размеру
			for (size_t i = 0; i < allocated_memory_blocks.size(); i++)
			{
				if (allocated_memory_blocks[i].current_empty_element == nullptr)
				{
					continue;
				}

				auto start_of_iterate_element = allocated_memory_blocks[i].current_empty_element;
				auto iterate_element = start_of_iterate_element;

				std::size_t counter = 1;

				auto start = allocated_memory_blocks[i].start_element;

				for (;;)
				{
					if (start + (*iterate_element) * sizeof(T) - iterate_element == sizeof(T))   //Если следующий элемент в памяти лежит рядом
					{
						counter += 1;                                                //То обновляем размер свободной памяти
						iterate_element = iterate_element + sizeof(T);            //И переходим к следующему элементу
					}
					else if (*iterate_element == 0)
					{
						if ( counter == n) {
							min_value.first = n;
							min_value.second = start_of_iterate_element;
							//std::cout << "We have free memory for, elements: " << counter << "; Block: " << std::endl;

						}
						else
						{
							check_min_memory(counter, n, min_value, start_of_iterate_element, iterate_element);
						}
						num_of_block = i;
						break;
				
					}
					else                                                           //Если вдруг элементы не лежат в памяти друг с другом, то проверяем , какого размера память у нас в счетчике
					{
						if (counter == n) {                                         //Если ее размер равен размеру, который просит контейнер, то отдаем этот участок памяти
							min_value.first = n;
							min_value.second = start_of_iterate_element;
							//std::cout << "We have free memory for, elements: " << counter << "; Block: " << std::endl;
							num_of_block = i;
							break;
						}
						else
						{
							check_min_memory(counter, n, min_value, start_of_iterate_element, iterate_element);
							start_of_iterate_element = start + *(iterate_element) * sizeof(T);
							iterate_element = start_of_iterate_element;
						}
					}
				}
			}
				
				//Далее проверяем,есть ли у нас из выделенной памяти свободная, если  памяти у нас хватает, то отдаем данный указатель контейнеру, если нет, то создаем выделяем новый участок памяти

			if (min_value.first != 0)
			{
				auto empty = allocated_memory_blocks[num_of_block].current_empty_element;
				auto start = allocated_memory_blocks[num_of_block].start_element;
				if (empty == min_value.second)
				{
					allocated_memory_blocks[num_of_block].current_empty_element = check_free_space(min_value.second, n);
				}
				else
				{
					auto before_allocated_element = allocated_memory_blocks[num_of_block].current_empty_element;
					while ((start + (*before_allocated_element)*sizeof(T)) != min_value.second)
					{
						before_allocated_element = start + (*before_allocated_element) * sizeof(T);
					}
					*before_allocated_element = min_value.second[(n - 1) * sizeof(T)];
				}
				
				return reinterpret_cast<T*>(min_value.second);

			}
			else
			{
				//std::cout << "We have not enoght free memory . Give memory. Size: " << n << "; Give: " << num_elements << "; Ost: " << num_elements - n << ";" << std::endl;
				begin_of_block = reinterpret_cast<unsigned char*>(malloc(used_memory)); //Выделяем память, на начало нашей памяти смотрит указалель элементов
				memory_allocate(begin_of_block, num_elements); // Разбиваем всю выделеную память на элементы, каждый элемент хранит указатель на следующий элемент
				blocks_count += 1;

				current_empty_element = check_free_space(begin_of_block, n);

				Memory_Block block{ begin_of_block ,(begin_of_block + used_memory),current_empty_element };

				allocated_memory_blocks.push_back(block);
				return reinterpret_cast<T*>(begin_of_block);
				}
			
		}
        
        
	};
    void deallocate (T* p, std::size_t n) {
		auto pointer_to_dealloc = reinterpret_cast<unsigned char*>(p);
		Memory_Block block_dealloc {nullptr,nullptr,nullptr};
		int block_number = 0;

		//Проверяем, есть ли вообще выделенная память нашим аллокатором

		if (allocated_memory_blocks.size() == 0)
		{
			return;
			//throw std::bad_alloc();
		}
		//Выделяли ли мы память по данному указателю?
		for (size_t i = 0; i < allocated_memory_blocks.size(); i++)
		{
			auto start = allocated_memory_blocks[i].start_element;
			auto last = allocated_memory_blocks[i].last_element;

			if ((start <= pointer_to_dealloc) && (last > pointer_to_dealloc))
			{
				block_dealloc = allocated_memory_blocks[i];
				block_number = i;
				break;
			}
		}
		if (block_dealloc.start_element == nullptr)
		{
			//return;
			throw std::bad_alloc();
		}

		// Если не попадаем в начальный байт блока, то сдвигаем указатель деаллокатора на него
		if ((pointer_to_dealloc - block_dealloc.start_element) % 4 != 0) {
			pointer_to_dealloc = pointer_to_dealloc - ((pointer_to_dealloc - block_dealloc.start_element) % sizeof(T));
		}
		//Проверяем, не выходят ли адреса за пределы аллоцированной памяти
		if ((pointer_to_dealloc + (n-1)*sizeof(T))>= block_dealloc.last_element)
		{
			throw std::bad_alloc();
		}
	
		//Если память в блоке полностью заполнена
		if (block_dealloc.current_empty_element  == nullptr)
		{
			for (int i = n - 1; i > -1; i--)
			{
				auto element = pointer_to_dealloc + i * sizeof(T);
				if (block_dealloc.current_empty_element == nullptr)
				{
					*(element) = 0;
				}
				else
				{
					*(element) = (block_dealloc.current_empty_element - block_dealloc.start_element) / sizeof(T);
				}

				block_dealloc.current_empty_element = element;
			}
		}
		//Если указатель на деаллоцированный участок меньше, чем на самый первый пустой участок
		else if (block_dealloc.current_empty_element > pointer_to_dealloc)
		{
			for (int i = n-1; i > -1; i--)
			{
				auto element = pointer_to_dealloc + i * sizeof(T);
				if (block_dealloc.current_empty_element == nullptr)
				{
					*(element) = 0;
				}
				else
				{
					*(element) = (block_dealloc.current_empty_element- block_dealloc.start_element)/sizeof(T);
				}
				 
				 block_dealloc.current_empty_element = element;
			}
		}
		//Если указатель на деаллоцированный участок больше, чем на самый первый пустой участок

		else
		{
			//Если больше
			//Первоначально проверяем, не деаллоцируем ли мы память, которая находится в самом конце нашей выделенной памяти 
			auto element = block_dealloc.current_empty_element;
			while (*element != 0)
			{
				element = block_dealloc.start_element + *(element) * sizeof(T);
			}
			//Если указатель последнего свободного участка меньше, чем указатель на деаллоцированную память
			if (element < pointer_to_dealloc)
			{
				for (int i = n - 1; i > -1; i--)
				{
					auto element_d = pointer_to_dealloc + i * sizeof(T);
					if (i == (int)n - 1)
					{
						*(element_d) = 0;
					}
					else
					{
						*(element_d) = ((pointer_to_dealloc - block_dealloc.start_element) / sizeof(T) + 1);
					}
					*element = (pointer_to_dealloc - block_dealloc.start_element) / sizeof(T);
				}
			}

			//Если указатель последнего участка больше, чем указатель на деаллоцированную память, то ищем те два свободных блока, между которыми попадает деаллоцированный указатель
			else
			{
				auto prev_element = block_dealloc.current_empty_element;
				auto next_element = block_dealloc.start_element + (*block_dealloc.current_empty_element) * sizeof(T);

				while (next_element < pointer_to_dealloc)
				{
					prev_element = next_element;
					next_element = block_dealloc.start_element + *(next_element) * sizeof(T);

				}

				for (int i = n - 1; i > -1; i--)
				{
					auto element_d = pointer_to_dealloc + i * sizeof(T);
					if (i == (int)n - 1)
					{
						*element_d = (next_element - block_dealloc.start_element) / sizeof(T);
					}
					else
					{
						*element_d = (element_d - block_dealloc.start_element) / sizeof(T) +1;
						if (i == 0) {
							*prev_element = (element_d - block_dealloc.start_element) / sizeof(T);
						}

					}

				}

			}
		}
 		allocated_memory_blocks[block_number] = block_dealloc;

		for (std::size_t i = 0; i < allocated_memory_blocks.size(); i++)
		{
			if (allocated_memory_blocks[i].current_empty_element==nullptr){
				continue;
			}
			int size_block = (allocated_memory_blocks[i].last_element - allocated_memory_blocks[i].start_element)/sizeof(T);
			int counter = 1;
			auto element = allocated_memory_blocks[i].current_empty_element;
			while (*(element)!=0)
			{
				counter += 1;
				element = allocated_memory_blocks[i].start_element + *(element) * sizeof(T);
			}
			if (counter == size_block)
			{
				std::free(allocated_memory_blocks[i].start_element);
				allocated_memory_blocks.erase(allocated_memory_blocks.begin() +  i);
				break;
			}
		}
	
	}

    template< class U >
    struct rebind
    {
        typedef CustomAllocator<U> other;
	};

	unsigned char* check_free_space(unsigned char* block, std::size_t n) {
		if (*(block + sizeof(T)*(n - 1)) == 0) {
			return nullptr;
		}
		else
		{
			return (block + sizeof(T)*n);
		}
	}
    
	void check_min_memory(int counter, int n, std::pair<int, unsigned char*>& min_pair, unsigned char* start_of_iterate_element,unsigned char* iterate_element) {
		if (((counter > n) && (min_pair.first == 0)) || ((counter > n) && (min_pair.first != 0)&&(counter < min_pair.first)))             // Если размер участка больше, и до этого мы не находили другие участки, то записываем пару размер/указатель
		{
			min_pair.first = counter;
			min_pair.second = start_of_iterate_element;
			start_of_iterate_element = iterate_element + *(iterate_element);
			//std::cout << "We have free memory for, elements: " << counter << "; Block: " << std::endl;
			counter = 0;
		}
	
	}
    void memory_allocate(unsigned char* begin, std::size_t num_element){
        for (size_t i = 0; i < num_element-1 ; i++)
        {
			begin[i * sizeof(T)] = i+1;
            //std::cout << (void*)(begin + (i * sizeof(T))) << "->" << (int)*(begin + (i * sizeof(T))) << std::endl;

           
        }
		begin[(num_element-1) * sizeof(T)] = 0;
//		std::cout << (void*)(begin + ((num_element - 1) * sizeof(T))) << "->" << (int)*(begin + ((num_element - 1) * sizeof(T))) << std::endl;
//        std::cout << element << "->" <<  element->next_element << std::endl;
  
    }

	private:
		std::size_t pow(std::size_t num) {
			std::size_t i = 1;
			while (i < num)
			{
			i *= 2;
			}
		return i;
		}
		
		int blocks_count = 0;
		const int max_size = 256;
		//std::vector<std::pair<unsigned char*, unsigned char*>> allocated_memory_blocks;
		std::vector<Memory_Block> allocated_memory_blocks;
};

template <class T, class U>
constexpr bool operator== (const CustomAllocator<T>& a1, const CustomAllocator<U>& a2) noexcept
{
	return a1.pool == a2.pool;
}

template <class T, class U>
constexpr bool operator!= (const CustomAllocator<T>& a1, const CustomAllocator<U>& a2) noexcept
{
	return a1.pool != a2.pool;
}
