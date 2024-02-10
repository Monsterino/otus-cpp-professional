#include "parser.h"

#include <string>
#include <string_view>
#include <iostream>


Parser::Parser(std::shared_ptr<Accumulator> accumulator):accumulator_(accumulator) {}

void Parser::parse(const char* data, std::size_t size) {
	
	std::string_view stringtext(data, size);
	std::string command;

	for (auto el: stringtext)
	{
		if (el == '\n' && command.size())
		{
			accumulator_->add_command(command);
			command.clear();
		}
		else if (el != '\n'){
			command += el;
		}
	}
	if (command.size())
	{
		accumulator_->add_command(command);
	}

}
