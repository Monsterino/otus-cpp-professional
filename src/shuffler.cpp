#include "shuffler.h"
#include <cmath>
#include <algorithm>

Shuffler::Shuffler(int reduce_num):reduce_num_(reduce_num){}

 std::vector<std::pair<std::string,std::vector<int>>> Shuffler::run(std::vector<std::vector<std::pair<std::string, int>>>& data) {
    if(data.empty()) {
        return {};
    }


    std::vector<std::vector<std::pair<std::string,int>>> output_vector;
    std::vector<std::pair<std::string,std::vector<int>>> shuffled_vector;


    std::string min_key;
    int summary_count_elements = 0;


    for (auto map : data) {
        summary_count_elements += map.size();
    }

    std::vector<std::pair<std::string,int>> cur_vec;
    std::vector<int> vec_of_counts;

    while (!data.empty()){
        min_key = data.begin()->begin()->first;
        for (auto map : data) {
            auto first_element = map.begin()->first;
            if (first_element < min_key) {
                min_key = first_element;
            }
        }


        for (auto& map : data) {
            if (map.begin()->first!=min_key) {
                continue;
            }
            vec_of_counts.push_back(map.begin()->second);
            map.erase(map.begin(),map.begin()+1);

        }

        shuffled_vector.push_back(std::make_pair(min_key,vec_of_counts));
        vec_of_counts.clear();
        empty_remover(data);
    }
    return shuffled_vector;
}

void Shuffler::empty_remover(std::vector<std::vector<std::pair<std::string,int>>>& vec) {
    auto it = vec.begin();
    while (it != vec.end()) {
        if (it->empty()) {
            it = vec.erase(it);
        }
        else {
            it++;
        }
    }

}
