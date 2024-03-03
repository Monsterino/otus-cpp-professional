#include "accumulator.h"
#include <iostream>
#include <chrono>
#include <string>

//Accumulator::Accumulator(std::size_t bulk):static_state_(true), current_depth_(0){}
Accumulator::Accumulator(std::size_t bulk) : pusher_(Pusher::get_Puser(bulk)), static_state_(true), current_depth_(0) {}


Accumulator::~Accumulator() {
	pusher_->disconnect();
	if (!pusher_->get_connections()) {
		add_command("EOF");
	}
}

void Accumulator::add_command(const std::string& command) {
	
	if (command == "{")
	{
		if (static_state_)
		{
			pusher_->push(command_block_, !static_state_);

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
				pusher_->push(command_block_, !static_state_);
				static_state_ = true;
			}
		}

	}
	else if (command == "EOF")
	{
		if (static_state_)
		{
			pusher_->push(command);
		}

	}
	else
	{
		if (static_state_)
		{
			pusher_->push(command);
		}

		else
		{
			command_block_.push_back(command);
		}
	}

}


