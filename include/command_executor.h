#pragma once

#include"handlers.h"

class Command_Executor {
public:
	Command_Executor() { handlers.reserve(3); };
	Command_Executor(std::initializer_list<Handler*> list) :handlers(list) { handlers.reserve(3); };
	~Command_Executor();

	void push_handler(Handler* handler);
	void pass(const std::string cmd);

private:
	std::vector<Handler*> handlers;

};

