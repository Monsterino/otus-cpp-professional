#pragma once
#include <string>
#include <vector>
#include <iostream>


std::string text_configurator(const std::vector<std::string>& vec, bool endl = true);

std::vector<std::string> splitter(std::string& text, char delimeter);

void replacer(std::string& text, std::string w_to_replace, std::string new_word);

