#include "handlers.h"


void ExecuteHandler::process_command(const std::string& command) {
	if (accumulator.get_ready_to_execute() && (accumulator.get_num_commands()))
	{
		std::string sep = "";
		std::cout << "bulk: ";
		for (auto cmd : accumulator.return_commands())
		{
			std::cout << sep << cmd;
			sep = ", ";
		}
		std::cout << std::endl;
		accumulator.clear();
		accumulator.change_ready_state(false);
	}
	if (command=="EOF")
	{
		accumulator.clear();
		accumulator.change_ready_state(false);

	}
}
