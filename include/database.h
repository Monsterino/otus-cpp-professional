#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#include "table.h"
#include "error_codes.h"


class Database;
typedef std::string(Database::*command_type)(std::vector<std::string>& args);


class Database
{
public:
	Database();
	std::string execute_command(std::string& command);
	std::string insert_command(std::vector<std::string>& args);
	std::string truncate_command(std::vector<std::string>& args);
	std::string intersection_command(std::vector<std::string>& args);
	std::string symmetric_difference_command(std::vector<std::string>& args);

	static Database* get_Database();

	std::map<std::string, std::shared_ptr<Table>> get_tables();

private:
	std::map<std::string, command_type> command_type_;
	std::map<std::string, std::shared_ptr<Table>> tables_;

};

 