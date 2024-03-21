#include "splitter.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <fstream>


splitter::splitter(int blocks):blocks_count_(blocks) {}

std::vector<std::shared_ptr<FileReader>> splitter::split(const std::string &path) {

    std::filesystem::directory_entry file(path);

    uintmax_t filesize = file.file_size();

    int rest_blocks = blocks_count_;
    uintmax_t begin_block = 0;
    uintmax_t block_lenght = 0;
    uintmax_t rest_filesize = filesize;

    char break_symbol = '\n';
    char sm;
    std::ifstream data(path,std::fstream::binary);
    for (int i = 0; i < blocks_count_; i++) {
        rest_blocks = blocks_count_ - i;
        block_lenght = std::ceil(double(rest_filesize)/rest_blocks);
        data.seekg(block_lenght,data.cur);
        do
        {
            data.get(sm);
            if (!data) {
                break;
            }
            block_lenght++;
        }
        while (sm!=break_symbol);
        std::string path_ = path;
        split_stream.emplace_back(std::shared_ptr<FileReader>(new FileReader(path_,begin_block,block_lenght)));
        rest_filesize-= block_lenght;
        begin_block += block_lenght;
        if (!rest_filesize) {
            break;
        }
    }

    return split_stream;
}
