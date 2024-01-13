#include "handlers.h"

void LoggerHandler::process_command([[maybe_unused]] const std::string& command){
	
	if (accumulator.get_ready_to_execute() && (accumulator.get_num_commands()))
	{
			filename = std::string("bulk") + std::to_string(accumulator.get_time()) + std::string(".log");
		std::ofstream logfile;
		logfile.open(filename);

		std::string sep = "";
		logfile << "bulk: ";
		for (auto cmd : accumulator.return_commands())
		{
			logfile << sep << cmd;
			sep = ", ";
		}
		logfile.close();
		
	}
}

