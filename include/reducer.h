#pragma once
#include <vector>
#include <string>

class Reducer {
public:
    using Iterator = std::vector<std::pair<std::string,std::vector<int>>>::iterator;
    Reducer(int num_reducers);
    void run(std::vector<std::pair<std::string,std::vector<int>>>& shuffled_data);
private:
    int num_reducers_;
};
