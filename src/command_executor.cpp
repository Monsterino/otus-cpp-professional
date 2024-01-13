#include "command_executor.h"


Command_Executor::~Command_Executor() {
	for (auto handler : handlers)
	{
		delete handler;
	}
}


void Command_Executor::push_handler(Handler* handler) {
	handlers.push_back(handler);
}

void Command_Executor::pass(const std::string cmd) {
	for (auto handler : handlers)
	{
		handler->process_command(cmd);
	}
}
