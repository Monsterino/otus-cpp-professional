#include "allocator.h"
#include "n_container.h"

int main(){
    std::map<int,int> map_std_allocator;
    std::map<int,int,std::less<int>,CustomAllocator<std::pair<const int,int>>> map_custom_allocator;
    N_Container<int> container_std_allocator;
    N_Container<int,CustomAllocator<int>> container_custom_allocator;

    int factorial = 1;
      for (size_t i = 0; i < 10; i++)
    {
       map_std_allocator[i] = factorial;
       map_std_allocator.insert({i,factorial});

       map_custom_allocator[i] = factorial;
       map_custom_allocator.insert({i,factorial});

       container_std_allocator.push_back(i);
       container_custom_allocator.push_back(i);

       factorial*= (i+1);
    }
    
    for (const auto& [key, value] : map_std_allocator)
        std::cout <<  key << " " << value << std::endl;

    for (const auto& [key, value] : map_custom_allocator)
        std::cout <<  key << " " << value << std::endl;

    for (size_t i = 0; i < 10; i++){
        std::cout << container_std_allocator[i] <<std::endl;
    }

   for (size_t i = 0; i < 10; i++){
        std::cout << container_custom_allocator[i] <<std::endl;
   }
};