#pragma once

#include "accumulator.h"
#include <fstream>
#include <vector>
#include <string>

class Parser
{
public:
	Parser(std::shared_ptr<Accumulator> accumulator);
	~Parser() = default;
	void parse(const char* data, std::size_t size);
	
	std::shared_ptr<Accumulator> get_accumulator();

private:
	std::shared_ptr<Accumulator> accumulator_;
};


