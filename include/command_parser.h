#pragma once
#include "database.h"

#include <string>
#include <queue>

class Parser
{
public:
	Parser(char deliter);
	void prepare_commands(std::string input_data);
	std::string execute_command();
	bool has_commands();

private:
	Database* base_;
	char deliter_;
	std::queue<std::string> commands_;
};

