#pragma once

#include <fstream>
#include "accumulator.h"
#include <parser.h>
#include <writer.h>
#include <logger.h>


class Context{

public:
	Context(std::size_t bulk);
	~Context() = default;

	void receive(const char* data, std::size_t size);
	
private:
	int id_;
	std::shared_ptr<Parser> parser_;
	std::shared_ptr<Logger> logger_;
	std::shared_ptr<Writer> writer_;
	std::shared_ptr<Accumulator> accumulator_;
};

//int Context::ID = 0;

