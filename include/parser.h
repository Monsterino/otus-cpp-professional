#pragma once

#include "accumulator.h"
#include <fstream>
#include <vector>

class Parser
{
public:
	Parser(std::shared_ptr<Accumulator> accumulator);
	~Parser() = default;
	void parse(const char* data, std::size_t size);

private:
	std::shared_ptr<Accumulator> accumulator_;
};


