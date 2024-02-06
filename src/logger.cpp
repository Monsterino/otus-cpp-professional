#include "logger.h"
#include <iostream>

void Logger::write(std::vector<std::string>& vec, std::string& time) {
	
	std::string sep = "";
	std::cout << "bulk: ";
	for (std::vector<std::string>::iterator it = vec.begin(); it !=vec.end(); it++)
	{
		std::cout << sep << *it;
		sep = ", ";
	}
	std::cout << std::endl;
};

