#include "database.h"
#include "utils.h"

#include <string>

Database::Database() {
	command_type_.emplace("INSERT", &Database::insert_command);
	command_type_.emplace("TRUNCATE", &Database::truncate_command);
	command_type_.emplace("INTERSECTION", &Database::intersection_command);
	command_type_.emplace("SYMMETRIC_DIFFERENCE", &Database::symmetric_difference_command);

	tables_.emplace("A", std::shared_ptr<Table>(new Table));
	tables_.emplace("B", std::shared_ptr<Table>(new Table));

};

std::string Database::execute_command(std::string& command) {

	if (command.empty())
	{
		return Message::code_to_string(Message(Message::Code::ERR, std::string("Command is empty")));
	}

	std::vector<std::string> parsed_commands = splitter(command, ' ');
	std::string cmd = parsed_commands[0];
	parsed_commands.erase(parsed_commands.begin());
	
	auto el = command_type_.find(cmd);
	if (el == command_type_.end())
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Unknown command"));
	}
	auto fun = el->second;

	std::string result = std::invoke(fun, this, parsed_commands);
	return result;
}

std::string Database::insert_command(std::vector<std::string>& args) {
	int index;
	
	if (args.size() != 3)
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Uncorrect arguments"));
	}
	auto table_it = tables_.find(args[0]);
	
	if (table_it == tables_.end())
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Unknown table " + args[0]));
	}
	
	try
	{
		index = std::stoi(args[1]);
	}
	catch (const std::exception&)
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Uncorrect index"));
	}
	

	auto table = table_it->second;

//	std::unique_lock lock(m_mutex);
	std::string message = table->insert(index, args[2]);
	
	return message;
}

std::string Database::truncate_command(std::vector<std::string>& args) {
	if (args.size() != 1)
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Uncorrect arguments"));
	}
	auto table_it = tables_.find(args[0]);

	if (table_it == tables_.end())
	{
		return Message::code_to_string(Message(Message::Code::ERR, "Unknown table"));
	}
	auto table = table_it->second;
//	std::unique_lock lock(m_mutex);

	std::string message = table->clear();
	return message;

}

std::string Database::intersection_command(std::vector<std::string>& args) {
//	std::unique_lock lock(m_mutex);

	std::string message = tables_["A"]->intersection(*tables_["B"]);
	return message;
}

std::string Database::symmetric_difference_command(std::vector<std::string>& args) {
//	std::unique_lock lock(m_mutex);

	std::string message = tables_["A"]->symmetric_difference(*tables_["B"]);
	return message;
}

Database* Database::get_Database() {
	static Database database;
	return &database;
}


std::map<std::string, std::shared_ptr<Table>> Database::get_tables() {
	return tables_;

}