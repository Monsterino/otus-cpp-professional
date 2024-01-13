#pragma once

#include<string>
#include<vector>
#include<ctime>

class Bulk_Accumulator {
public:
	Bulk_Accumulator(int bulk_size);
	void process_command(const std::string& command);
	void change_ready_state(bool state);
	void clear();

	bool get_ready_to_execute() const;
	time_t get_time() const;
	std::vector<std::string> return_commands() const;
	int get_num_commands() const;

private:
	std::vector<std::string> commands;
	bool ready;
	time_t time;

};

