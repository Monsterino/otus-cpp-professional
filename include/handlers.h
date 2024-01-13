#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "bulk_command.h"




class Handler {
public:
	virtual void process_command([[maybe_unused]] const std::string& command) {};
	virtual ~Handler() = default;

};

class MainHandler : public Handler {
public:
	MainHandler(int bulk_size, Bulk_Accumulator& accumulator) :bulk_size(bulk_size), accumulator(accumulator), is_static(true),depth(0) {};
	void process_command(const std::string& command) override;
private:
	int bulk_size;
	Bulk_Accumulator& accumulator;
	bool is_static;
	int depth;
};

class LoggerHandler : public Handler {
public:
	LoggerHandler(const Bulk_Accumulator& accumulator) :accumulator(accumulator) {};
	void process_command(const std::string& command) override;
private:
	const Bulk_Accumulator& accumulator;
	std::string filename;
	
};

class ExecuteHandler : public Handler {
public:
	ExecuteHandler(Bulk_Accumulator& accumulator) :accumulator(accumulator) {};
	void process_command(const std::string& command) override;
private:
	Bulk_Accumulator& accumulator;
};
