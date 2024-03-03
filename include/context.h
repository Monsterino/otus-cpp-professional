#pragma once

#include <fstream>

#include "parser.h"

class Context{

public:
	Context(std::size_t bulk);
	~Context() = default;

	void receive(const char* data, std::size_t size);
	
private:
	std::shared_ptr<Parser> parser_;
	std::shared_ptr<Accumulator> accumulator_;
};


