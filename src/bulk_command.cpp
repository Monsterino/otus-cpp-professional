#include "bulk_command.h"

#include<chrono>


Bulk_Accumulator::Bulk_Accumulator(int bulk_size) :ready(false),time(0) {
	commands.reserve(bulk_size);
}

void Bulk_Accumulator::process_command(const std::string& command) {
	commands.push_back(command);
	if (get_num_commands()==1)
	{
		auto now = std::chrono::system_clock::now();
		time = std::chrono::system_clock::to_time_t(now);
	}
}

void Bulk_Accumulator::change_ready_state(bool state) {
	ready = state;
}

void  Bulk_Accumulator::clear() {
	commands.clear();
}

bool Bulk_Accumulator::get_ready_to_execute() const {
	return ready;
}

time_t Bulk_Accumulator::get_time() const {
	return time;
}

std::vector<std::string> Bulk_Accumulator::return_commands() const {
	return commands;
}

int Bulk_Accumulator::get_num_commands() const {
	return commands.size();
}
