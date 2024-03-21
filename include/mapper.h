#pragma once


#include <vector>
#include <string>
#include <functional>
#include <memory>
//class FileReader;
#include "../include/reader.h"

class Mapper {
public:
    Mapper(int num_mapper, std::vector<std::shared_ptr<FileReader>>& readers, std::function<std::pair<std::string,int>(const std::string&)>& func);
    ~Mapper() = default;
    std::vector<std::vector<std::pair<std::string,int>>> run();
private:
    int num_mapper_;
    std::function<std::pair<std::string,int>(std::string)> map_function_;
    std::vector<std::shared_ptr<FileReader>>& readers_;
};
