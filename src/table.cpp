#include "table.h"
#include "error_codes.h"

std::string Table::insert(int id, std::string data) {
	std::unique_lock<std::shared_mutex> lock(mutex);

	auto element = data_.find(id);
	if (element != data_.end())
	{
		return Message::code_to_string(Message(Message::Code::ERR, std::string("duplicate ") + std::to_string(id)));
	}
	else
	{
		data_.emplace(id, data);
		return Message::code_to_string(Message(Message::Code::OK, std::string("OK")));
	}
}

std::string Table::intersection(Table& second_table) {

	auto it_a_begin = data_.begin();
	auto it_a_end = data_.end();
	auto it_b_begin = second_table.get_data().begin();
	auto it_b_end = second_table.get_data().end();
	std::string data;

	while ((it_a_begin != it_a_end) && (it_b_begin != it_b_end))
	{
		if (it_a_begin->first < it_b_begin->first)
		{
			it_a_begin++;
		}
		else
		{
			if (!(it_b_begin->first< it_a_begin->first))
			{
				data += std::to_string(it_b_begin->first) + "," + it_a_begin->second + "," + it_b_begin->second + "\n";
				it_a_begin++;
			}
			it_b_begin++;
		}

	}
	data += Message::code_to_string(Message(Message::Code::OK, "OK"));
	return data;
}

std::string Table::symmetric_difference(Table& second_table) {
	std::shared_lock<std::shared_mutex> left_lock(mutex, std::defer_lock);
	std::shared_lock<std::shared_mutex> right_lock(second_table.mutex, std::defer_lock);
	std::lock(left_lock, right_lock);


	auto it_a_begin = data_.begin();
	const auto it_a_end = data_.end();
	auto it_b_begin = second_table.get_data().begin();
	const auto it_b_end = second_table.get_data().end();
	std::string data;

	while (it_a_begin != it_a_end)
	{
		if (it_b_begin == it_b_end) {
			while (it_a_begin != it_a_end) {
				data += std::to_string(it_a_begin->first) + "," + it_a_begin->second + ",\n";
				it_a_begin++;
			}
			if (it_a_begin == it_a_end)
			{
				break;
			}
			
		}
		if (it_a_begin->first < it_b_begin->first)
		{
			data += std::to_string(it_a_begin->first) + "," + it_a_begin->second + ",\n";
			it_a_begin++;
		}
		else
		{
			if (it_a_begin->first > it_b_begin->first)
			{
				data += std::to_string(it_b_begin->first) + ",," + it_b_begin->second + "\n";
			}
			else {
				it_a_begin++;
			}
			it_b_begin++;
		}
	}
	while (it_b_begin != it_b_end)
	{
		data += std::to_string(it_b_begin->first) + ",," + it_b_begin->second + "\n";
		it_b_begin++;
	}

	data += Message::code_to_string(Message(Message::Code::OK, "OK"));
	return data;
}
std::string	Table::clear() {
	std::unique_lock<std::shared_mutex> lock(mutex);
	data_.clear();
	return Message::code_to_string(Message(Message::Code::OK, "OK"));
}
const std::map<int, std::string>& Table::get_data() {
	return data_;
}
