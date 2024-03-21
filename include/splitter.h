#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../include/reader.h"

class splitter {
public:
    splitter(int blocks);
    std::vector<std::shared_ptr<FileReader>> split(const std::string & path);
private:
    int blocks_count_;
    std::vector<std::shared_ptr<FileReader>> split_stream;
};
