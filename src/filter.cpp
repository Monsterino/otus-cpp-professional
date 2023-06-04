#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <cmath>
#include <future>
#include <algorithm>

#include "filter.h"

void filter::add_ip(long long ip){
    ip_container.push_back(ip);
}

void filter::sort(bool reverse){
    long long* sorted_container = ip_container.data();
    long long* temp = new long long[ip_container.size()];
    create_branch(sorted_container,temp,ip_container.size(),1,2);
    if (reverse == true)
    {
        for (size_t i = 0; i < ip_container.size(); i++)
        {
            temp[i] = sorted_container[ip_container.size()-i-1];
        }
    std::memcpy(sorted_container,temp,ip_container.size()*sizeof(long long));

    }
    

    delete[] temp;
}

void filter::sort(std::vector<long long>& container,bool reverse){
    long long* sorted_container = container.data();
    long long* temp = new long long[container.size()];
    create_branch(sorted_container,temp,container.size(),1,2);

    if (reverse == true)
    {
        for (size_t i = 0; i < container.size(); i++)
        {
            temp[i] = sorted_container[container.size()-i-1];
        }
    std::memcpy(sorted_container,temp,container.size()*sizeof(long long));

    }

    delete[] temp;
}

void filter::create_branch(long long* array,long long* temp,int container_size,int current_depth,int depth){
    if (container_size <2){
        return;
    }
    int middle = container_size/2;

    auto left_array = array;
    auto left_temp = temp;
    auto left_size = middle;

    auto right_array = array+middle;
    auto rigth_temp = temp+middle;
    auto right_size = container_size - middle;

    if (current_depth<=depth)
    {
        std::thread thread_1(&filter::create_branch,this, left_array, left_temp, left_size, current_depth+1, depth);
        std::thread thread_2(&filter::create_branch,this, right_array, rigth_temp, right_size, current_depth+1, depth);

        thread_1.join();
        thread_2.join();

    }
    else
    {
        create_branch(left_array,left_temp,left_size,current_depth+1,depth);
        create_branch(right_array,rigth_temp,right_size,current_depth+1,depth);
    }
    int left_massive_index = 0, right_massive_index = 0, result_massive_index = 0;
    while (left_massive_index<left_size && right_massive_index < right_size)
    {
        if (left_array[left_massive_index]<=right_array[right_massive_index])
        {
            temp[result_massive_index] = left_array[left_massive_index];
            left_massive_index +=1;
        }
        else
        {
            temp[result_massive_index] = right_array[right_massive_index];
            right_massive_index +=1;
        }
        result_massive_index+=1;

      
    }
    while (left_massive_index < left_size){
        temp[result_massive_index] = left_array[left_massive_index];
        result_massive_index++;
        left_massive_index++;
        }
        while (right_massive_index < right_size){
        temp[result_massive_index] = right_array[right_massive_index];
        result_massive_index++;
        right_massive_index++;
        }
        std::memcpy(array,temp,container_size*sizeof(long long));
    

    
}

void filter::search(const short& first,const short& second,const short& third,const short& fourth){
    std::vector<std::future<std::vector<long long>>> f;
    //std::future<std::vector<long long>> f;
    if (first!=-1)
    {
        f.push_back(std::async(std::launch::async,&filter::search_bytes,this, ip_container, first,0));
    }
    if (second!=-1)
    {
        f.push_back(std::async(std::launch::async,&filter::search_bytes,this, ip_container, second,1));
    }
    if (third!=-1)
    {
       f.push_back(std::async(std::launch::async,&filter::search_bytes,this, ip_container, third,2));
    }
    if (fourth!=-1)
    {
        f.push_back(std::async(std::launch::async,&filter::search_bytes,this, ip_container, fourth,3));
    }
    for (size_t i = 0; i < f.size(); i++)
    {
       f[i].wait();
    }
    
    if (f.size()==0)
    {
        std::cout<< "Проверьте, указали ли вы значения для фильтра" << std::endl;
    }
    
    else if (f.size()==1)
    {
        std::vector<long long> table = f[0].get();
        filter::sort(table, true);

        for (size_t i = 0; i < table.size(); i++)
        {
            std::vector<int> ip_mass;
            get_ip(table[i],ip_mass);
            std::cout << ip_mass[3] << "." << ip_mass[2] << "." << ip_mass[1] << "." << ip_mass[0] << std::endl;
        }
        
    }
    else 
    {
        std::vector<long long> table_first = f[0].get();
        filter::sort(table_first);

        for (size_t i = 1; i < f.size(); i++)
        {
            auto table = f[i].get();
            filter::sort(table);
            std::vector<long long> v_intersection;
            std::set_intersection(table_first.begin(), table_first.end(), table.begin(), table.end(), std::back_inserter(v_intersection));
            table_first = v_intersection;
            filter::sort(table_first,true);
        }
        for (size_t i = 0; i < table_first.size(); i++)
        {
            std::vector<int> ip_mass;
            get_ip(table_first[i],ip_mass);
            std::cout << ip_mass[3] << "." << ip_mass[2] << "." << ip_mass[1] << "." << ip_mass[0] << std::endl;
        }
    }
   
}

void filter::get_ips(){
    for (auto ip: ip_container){
        std::vector<int> ip_mass;
        get_ip(ip,ip_mass);
        std::cout << ip_mass[3] << "." << ip_mass[2] << "." << ip_mass[1] << "." << ip_mass[0] << std::endl;
                   
        }
}
//
//for (int i = 0; i < ip_container.size(); i++)
//    {
//        int first_byte = ip_container[i]/std::pow(1000,3);
//        int second_byte = (ip_container[i] - first_byte * std::pow(1000,3))/std::pow(1000,2);
//        int third_byte = (ip_container[i]- first_byte * std::pow(1000,3) - second_byte * std::pow(1000,2))/std::pow(1000,1);
//        int fourth_byte = (ip_container[i]- first_byte * std::pow(1000,3) - second_byte * std::pow(1000,2) - third_byte*std::pow(1000,1))/std::pow(1000,0);
//        //std::cout << sorted_container[i] <<std::endl;
//        //std::cout << ip_container[i] << std::endl;
//        std::cout << first_byte << "."<< second_byte << "."<< third_byte << "."<< fourth_byte << std::endl;
//    }
//


//std::vector<int> filter::search_bytes(std::vector<long long> array , short search_value,  int byte){
std::vector<long long> filter::search_bytes(const std::vector<long long>& array ,const short& search_value, const int& byte){
    std::vector<long long> ips;
    for(long long ip : array){
        std::vector<int> ip_mass;
        get_ip(ip,ip_mass);
        int current_byte = ip_mass[3-byte];
        if (current_byte == search_value)
        {
            ips.push_back(ip);
        }
    }
    return ips;
    
}

void filter::get_ip(const long long& ip, std::vector<int>& vec){
    vec.push_back (ip%1000);
    int temp = ip / 1000;  
    if (temp == 0 ) return;
    get_ip (temp, vec);
}
    
void filter::filter_any(const int& byte){
    for (auto ip: ip_container){
        std::vector<int> ip_mass;
        get_ip(ip,ip_mass);
        for (auto byte_ip: ip_mass){
            if (byte_ip == byte)
            {
               std::cout << ip_mass[3] << "." << ip_mass[2] << "." << ip_mass[1] << "." << ip_mass[0] << std::endl;
               break;
            }
            
        }
    }
}
