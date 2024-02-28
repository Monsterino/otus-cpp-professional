#pragma once
#include <fstream>
#include <vector>
#include <memory>

#include "handler.h"

class Accumulator
{
public:
	Accumulator(std::size_t bulk);
	virtual ~Accumulator();

	virtual void add_command(const std::string& command);
	void execute_block();

	void add_handler(std::shared_ptr<Handler> handler);

	std::vector<std::string> get_command_block();
	std::vector<std::shared_ptr<Handler>> get_handlers();

	bool get_state();
	int get_depth();


	void clear_block();
	void reset();

private:
	std::size_t bulk_;
	std::vector<std::string> command_block_;
	std::string first_command_time_;
	bool static_state_;
	int current_depth_;
	

	std::vector<std::shared_ptr<Handler>> handlers_;
};


