#include "reducer.h"
#include <fstream>
#include <thread>
#include <cmath>
#include <iostream>


Reducer::Reducer(int num_reducers):num_reducers_(num_reducers){};

void Reducer::run(std::vector<std::pair<std::string,std::vector<int>>>& shuffled_data) {

    auto multithread_reduce = [](int num_reducer,Iterator it,Iterator end) {
        std::string filename = "reducer_"+std::to_string(num_reducer)+ ".log";
        std::ofstream logfile;
        logfile.open(filename);

        while (it!=end) {
            auto data = it->first;
            auto nums = it->second;
            int num_elements = 0;
            for (auto el: nums) {
                num_elements+=el;
            }
            logfile << data << " " << std::to_string(num_elements) << "\n";
            it++;
        }
        logfile.close();
    };

    int num_of_classes = shuffled_data.size();
    if (num_of_classes<num_reducers_) {
        std::cout << "Size of shuffled data less than num of reducers" << std::endl;
        num_reducers_ = num_of_classes;
    }



    int rest_elements = num_of_classes;
    int rest_reducers = num_reducers_;

    Iterator begin = shuffled_data.begin();
    Iterator end = begin;
    std::vector<std::thread> th_redusers;


    while (rest_elements) {
        int elems = std::ceil(double(rest_elements)/ rest_reducers);
        end += elems;
        th_redusers.push_back(std::thread(multithread_reduce,num_reducers_-rest_reducers,begin,end));
        begin = end;
        rest_elements-=elems;
        rest_reducers--;

    }

    for (int i =0; i < th_redusers.size(); i++) {
        th_redusers[i].join();
    }
}