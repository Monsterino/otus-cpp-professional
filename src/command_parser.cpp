#include "command_parser.h"

#include "utils.h"

Parser::Parser(char deliter) : deliter_(deliter), base_(Database::get_Database()) {};

void Parser::prepare_commands(std::string input_data) {
	replacer(input_data, "\r", "");
	auto data = splitter(input_data, deliter_);
	for (auto el : data) {
		commands_.push(el);
	}
}

std::string Parser::execute_command() {
	auto command = commands_.front();
	commands_.pop();
	auto result = base_->execute_command(command);
	return result;
}

bool Parser::has_commands() {
	return commands_.size();
}