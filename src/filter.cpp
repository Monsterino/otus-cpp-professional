#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <cmath>
#include <future>
#include <algorithm>
#include <iterator>

#include "filter.h"

filter::filter(): _reverse(false) {};

void filter::add_ip(long long ip){
    ip_container.push_back(ip);
}

void filter::sort(bool reverse){
    _reverse = reverse;
    if (_reverse){
        std::sort(begin(ip_container), end(ip_container),std::greater<>());
    }
    else {
        std::sort(begin(ip_container), end(ip_container));
    }
    
}


void filter::sort(std::vector<long long>& container,bool reverse){
    if (reverse) {
        std::sort(begin(container), end(container), std::greater<>());
    }
    else {
        std::sort(begin(container), end(container));
    }
}


void filter::search(const short& first,const short& second,const short& third,const short& fourth){
    std::vector<std::future<std::vector<long long>>> f;
    //std::future<std::vector<long long>> f;

    std::cout << "Searching" << std::endl;
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
        show_ips(table);
        
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
            
        }
        if (_reverse)
        {
            std::reverse(table_first.begin(), table_first.end());
        }
        show_ips(table_first);

    }
    std::cout << "=============================" << std::endl;
}


void filter::show_ips() {
    get_ips(ip_container);
}
void filter::show_ips(std::vector<long long> vec) {
    get_ips(vec);
}


void filter::get_ips(std::vector<long long> vec){
    std::cout << "Show ips" << std::endl;
    for (auto ip: vec){
        std::vector<int> ip_mass;
        get_ip(ip,ip_mass);
        std::cout << ip_mass[3] << "." << ip_mass[2] << "." << ip_mass[1] << "." << ip_mass[0] << std::endl;
                   
        }
    std::cout << "=============================" << std::endl;
}





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
    std::cout << "Search for all" << std::endl;
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
