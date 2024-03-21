#pragma once

#include <string>
#include <fstream>


class FileReader {
public:
    FileReader(std::string& path,uintmax_t start, uintmax_t lenght);
    bool check_state();
    std::string get_data();
private:
    std::ifstream input_stream_;
    uintmax_t readed_bytes_;
    uintmax_t start_;
    uintmax_t lenght_;
    bool end_stream_;

    std::string path_;
};
