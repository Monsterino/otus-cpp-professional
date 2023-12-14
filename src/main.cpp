#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "filter.h"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

int main()
{
    try
    {
        filter my_filter;
        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            long long ip = 0;
            std::vector<std::string> v = split(line, '\t');
            
            auto ip_elements = split(v.at(0), '.');
            for (size_t i = 0; i < 4; i++)
            {
               ip = ip + std::pow(1000,3-i) * std::stoi(ip_elements[i]);  
            }
            my_filter.add_ip(ip);
            
        }

        my_filter.sort(false);
        my_filter.show_ips();
        my_filter.search(1);
        my_filter.search(46, 70);
        my_filter.filter_any(46);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
