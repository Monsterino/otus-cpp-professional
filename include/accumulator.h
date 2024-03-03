#pragma once
#include <fstream>
#include <vector>

#include "pusher.h"


class Accumulator
{
public:
	Accumulator(std::size_t bulk);
	~Accumulator();

	void add_command(const std::string& command);
	void execute_block();


private:
	std::vector<std::string> command_block_;
	bool static_state_;
	int current_depth_;
	Pusher* pusher_;

};


