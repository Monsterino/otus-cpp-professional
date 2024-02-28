#include <gtest/gtest.h>

#include "async.h"
#include "utils.h"
#include "barrier.h"
#include "test_classes.h"

#include <random>
#include <iostream>
#include <sstream>
#include <string>


TEST(Parser_test, Test_correct_command) {
	//ѕровер€ем, правильно ли работаем парсер команд
	int bulk = 2;
	auto test_accumulator = std::shared_ptr<Test_Accumulator>(new Test_Accumulator(bulk));
	auto test_parser = std::shared_ptr<Parser>(new Parser(test_accumulator));
	
	std::vector<std::string> expected_output;
	std::vector<std::string> output;

	std::string input_data{ "comm1\ncomm2\ncomm3" };
	expected_output = { "comm1","comm2","comm3" };
	test_parser->parse(input_data.data(), input_data.size());
	output = test_accumulator->get_commands();
	
	ASSERT_EQ(expected_output, output);
	test_accumulator->reset();


	input_data = { "comm1\ncomm2\ncomm3\nco4\n{\ntemp1\ntemp2\ntemp3\r_4\n}\n" };
	expected_output = { "comm1","comm2","comm3","co4","{","temp1","temp2","temp3_4","}" };
	test_parser->parse(input_data.data(), input_data.size());
	output = test_accumulator->get_commands();

	ASSERT_EQ(expected_output, output);
	test_accumulator->reset();


}

