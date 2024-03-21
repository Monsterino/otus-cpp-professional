#pragma once
#include <vector>
#include <string>

class Shuffler {
public:
    Shuffler(int reduce_num);
  //  std::vector<std::vector<std::pair<std::string,int>>> run(std::vector<std::vector<std::pair<std::string, int>>>& data);
    std::vector<std::pair<std::string,std::vector<int>>>  run(std::vector<std::vector<std::pair<std::string, int>>>& data);
    void empty_remover(std::vector<std::vector<std::pair<std::string,int>>>& vec);
private:
    int reduce_num_;
};
