#pragma once


#include <fstream>
#include <vector>
#include <memory>

#include "accumulator.h"


class Parser
{
public:
	Parser(std::shared_ptr<Accumulator> accumulator);
	~Parser() = default;
	void parse(const char* data, std::size_t size);

private:
	std::shared_ptr<Accumulator> accumulator_;
	std::string command;

};


