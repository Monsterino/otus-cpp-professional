#pragma once

#include <functional>
#include <string>

std::function<std::pair<std::string,int>(const std::string& line)> pair_of_line();
