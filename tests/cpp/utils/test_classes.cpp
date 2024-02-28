#include "test_classes.h"


void Test_handler::write(std::vector<std::string>& vec, std::string& time) {
	output_data = vec;
	output_time = time;
}

std::vector<std::string> Test_handler::get_data() {
	return output_data;
};
std::string Test_handler::get_time() {
	return output_time;
};


Test_Accumulator::Test_Accumulator(std::size_t bulk):Accumulator(bulk){}


void Test_Accumulator::add_command(const std::string& command) {
	input_commands_.push_back(command);

}

std::vector<std::string> Test_Accumulator::get_commands() {
	return input_commands_;
}


void Test_Accumulator::reset() {
	input_commands_.clear();
}