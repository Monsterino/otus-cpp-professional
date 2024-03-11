#include "utils.h"

#include <sstream>



void replacer(std::string& text, std::string w_to_replace, std::string new_word) {
	std::vector<size_t> positions;

	size_t pos = text.find(w_to_replace);
	while (pos != std::string::npos) {
		positions.push_back(pos);
		pos = text.find(w_to_replace, pos + 1);
	}
	if (positions.size())
	{
		size_t len = w_to_replace.length();
		for (int ps = positions.size() - 1; ps >= 0; ps--)
		{
			text.replace(positions[ps], len, new_word);
		}
	}
}

std::vector<std::string> splitter(std::string& text, char delimeter) {
	std::stringstream ss(text);
	std::string element;
	std::vector<std::string> elements;
	while (std::getline(ss, element, delimeter))
	{
		elements.push_back(element);
	}
	return elements;
}


