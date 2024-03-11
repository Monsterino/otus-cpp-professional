#pragma once

#include <string>
#include <map>
#include <mutex>
#include <shared_mutex>

class Table
{
public:
	Table() = default;

	std::string insert(int id, std::string data);
	std::string	clear();
	std::string intersection(Table& second_table);
	std::string symmetric_difference(Table& second_table);
	const std::map<int, std::string>& get_data();

private:
	std::map<int, std::string> data_;
	
protected:
	mutable std::shared_mutex mutex;
};

