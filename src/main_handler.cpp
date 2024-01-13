#include "handlers.h"

void MainHandler::process_command(const std::string& command){
	if (command == "{")
	{
		if (is_static)
		{
			accumulator.change_ready_state(true);
			is_static = false;
		}

		depth += 1;
	}
	else if (command == "}")
	{
		if (!is_static)
		{
			depth -= 1;

			if (depth == 0)
			{
				accumulator.change_ready_state(true);
				is_static = true;
			}
		}
	
	}
	else if (command == "EOF")
	{
		if (is_static)
		{
			accumulator.change_ready_state(true);
		}
		
	}
	else
	{
		accumulator.process_command(command);
		if (accumulator.get_num_commands()== bulk_size && is_static)
		{
			accumulator.change_ready_state(true);
		}
	}
}

