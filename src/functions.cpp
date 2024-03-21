#include "functions.h"

std::function<std::pair<std::string,int>(const std::string& line)> pair_of_line(){
    return [](const std::string& line) {
        return std::make_pair(line,1);
    };
}

