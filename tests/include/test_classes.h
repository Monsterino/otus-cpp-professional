#pragma once

#include "async.h"


class Test_handler : public Handler
{
public:
	Test_handler() = default;
	~Test_handler() = default;
	void write(std::vector<std::string>& vec, std::string& time) override;

	std::vector<std::string> get_data();
	std::string get_time();

private:
	std::vector<std::string> output_data;
	std::string output_time;
};


class Test_Accumulator : public Accumulator
{
public:
	Test_Accumulator(std::size_t bulk);
	~Test_Accumulator() = default;

	void add_command(const std::string& command) override;
	void reset();

	std::vector<std::string> get_commands();
	
private:
	std::vector<std::string> input_commands_;
};
