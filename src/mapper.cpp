#include "mapper.h"

#include <iostream>
#include <thread>

Mapper::Mapper(int num_mapper, std::vector<std::shared_ptr<FileReader>>& readers,
               std::function<std::pair<std::string, int>(const std::string&)> &func):num_mapper_(num_mapper),
                                                                                     readers_(readers),
                                                                                     map_function_(func){};


std::vector<std::vector<std::pair<std::string,int>>> Mapper::run() {
    if (readers_.size() < num_mapper_) {
        std::cout << "So much readers for current file" << std::endl;
        num_mapper_ = readers_.size();
    }
    std::vector<std::vector<std::pair<std::string,int>>> vec(num_mapper_);

    auto multithread_map = [*this](std::shared_ptr<FileReader> reader,  std::vector<std::pair<std::string,int>>& place) {
        std::vector<std::pair<std::string,int>> vec_mapper;
        std::vector<std::pair<std::string,int>> combined_vec;


        while (reader->check_state()) {
            vec_mapper.push_back(map_function_(reader->get_data()));
        }
        std::sort(vec_mapper.begin(),vec_mapper.end());

        std::string last_element = vec_mapper.begin()->first;
        int counter = 1;
        for (auto it = (vec_mapper.begin()+1);it !=vec_mapper.end();it++)
            if (it->first!=last_element) {
                combined_vec.push_back(std::make_pair(last_element,counter));
                last_element = it->first;
                counter = 1;
            }
            else {
                counter++;
            }
        combined_vec.push_back(std::make_pair(last_element,counter));
        place = combined_vec;

    };

    std::vector<std::thread> th_mappers;

    for (int i =0; i < num_mapper_; i++) {
       auto vec2 = vec[i];
       th_mappers.push_back(std::thread(multithread_map,std::ref(readers_[i]),std::ref(vec[i])));
    }
    for (int i =0; i < th_mappers.size(); i++) {
        th_mappers[i].join();
    }

    return  vec;
}


