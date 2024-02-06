#include "writer.h"
#include <iostream>

void Writer::write(std::vector<std::string>& vec, std::string& time) {
	std::string filename = std::string("bulk") + time + std::string(".log");
	std::ofstream logfile;
	logfile.open(filename);

	std::string sep = "";
	logfile << "bulk: ";

	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		logfile << sep << *it;
		sep = ", ";
	}
	logfile.close();

};

