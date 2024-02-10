#pragma once
#include <fstream>
#include <vector>
#include <memory>

#include "handler.h"

class Accumulator
{
public:
	Accumulator(std::size_t bulk);
	~Accumulator();

	void add_command(const std::string& command);
	void execute_block();

	void add_handler(std::shared_ptr<Handler> handler);

private:
	std::size_t bulk_;
	std::vector<std::string> command_block_;
	bool static_state_;
	int current_depth_;
	std::string first_command_time_;

	std::vector<std::shared_ptr<Handler>> handlers_;
};


