#include "accumulator.h"
#include <iostream>
#include <chrono>
#include <string>

Accumulator::Accumulator(std::size_t bulk):bulk_(bulk),current_depth_(0),static_state_(true), first_command_time_("0"){}

Accumulator::~Accumulator() {
	add_command("EOF");
}

void Accumulator::add_command(const std::string& command) {
	
	if (command == "{")
	{
		if (static_state_)
		{
			execute_block();
			static_state_ = false;
		}

		current_depth_ += 1;
	}
	else if (command == "}")
	{
		if (!static_state_)
		{
			current_depth_ -= 1;

			if (current_depth_ == 0)
			{
				execute_block();
				static_state_ = true;
			}
		}

	}
	else if (command == "EOF")
	{
		if (static_state_)
		{
			execute_block();
		}

	}
	else
	{
		if (!command_block_.size())
		{
			auto now = std::chrono::system_clock::now();
			first_command_time_ = std::to_string(std::chrono::system_clock::to_time_t(now));
		}

		command_block_.push_back(command);
		if (command_block_.size() == bulk_ && static_state_)
		{
			execute_block();	
		}

	}

}

void Accumulator::execute_block()  {

	if (command_block_.size())
	{
		for (auto handler: handlers_)
		{
			handler->write(command_block_, first_command_time_);
		}
	}
	command_block_.clear();
}	


void Accumulator::add_handler(std::shared_ptr<Handler> handler) {
	handlers_.push_back(handler);
}

std::vector<std::string> Accumulator::get_command_block() {
	return command_block_;
}

std::vector<std::shared_ptr<Handler>> Accumulator::get_handlers() {
	return handlers_;
}

bool Accumulator::get_state() {
	return static_state_;
};
int Accumulator::get_depth() {
	return current_depth_;
}

void Accumulator::clear_block(){
	command_block_.clear();
}


void Accumulator::reset() {
	current_depth_ = 0;
	static_state_ = true;
	first_command_time_="0";
	clear_block();
}