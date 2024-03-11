#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "error_codes.h"



std::vector<std::string> splitter(std::string& text, char delimeter);

void replacer(std::string& text, std::string w_to_replace, std::string new_word);

