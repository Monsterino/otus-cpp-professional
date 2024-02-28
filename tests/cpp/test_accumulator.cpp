#include <gtest/gtest.h>

#include "async.h"
#include "utils.h"
#include "test_classes.h"

#include <iostream>
#include <sstream>
#include <string>


void accumulator_adder(std::shared_ptr<Accumulator> test_accumulator, std::vector<std::string>& commands) {
	for (auto cmd: commands)
	{
		test_accumulator->add_command(cmd);
	}
}

TEST(Accumulator_test, check_depth) {
	//ѕровер€ем правильность глубины вложенности аккумул€тора
	int bulk = 2;
	auto test_accumulator = std::shared_ptr<Accumulator>(new Accumulator(bulk));

	ASSERT_EQ(test_accumulator->get_depth(), 0);


	std::vector<std::string> commands{ "a","{","b" };
	int expected_depth = 1;
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_depth(), expected_depth);
	test_accumulator->reset();

	commands = { "a","{","b","{","c" };
	expected_depth = 2;
	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_depth(), expected_depth);
	test_accumulator->reset();

	commands = { "a","{","b","{","c","}" };
	expected_depth = 1;
	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_depth(), expected_depth);
	test_accumulator->reset();


	commands = { "a","{","b","{","c","}", "e", "}" };
	expected_depth = 0;
	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_depth(), expected_depth);
	test_accumulator->reset();

}

TEST(Accumulator_test, check_static) {
	//ѕровер€ем правильность определени€ размера блока команд, с которым мы работаем, статический или динамический блок
	int bulk = 2;
	auto test_accumulator = std::shared_ptr<Accumulator>(new Accumulator(bulk));

	bool expected_state = 1;

	ASSERT_EQ(test_accumulator->get_state(), 1);


	std::vector<std::string> commands{ "a","{","b" };
	expected_state = 0;
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_state(), expected_state);
	test_accumulator->reset();

	commands = { "a","{","b","{","c" };
	expected_state = 0;
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_state(), expected_state);
	test_accumulator->reset();

	commands = { "a","{","b","{","c","}" };
	expected_state = 0;
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_state(), expected_state);
	test_accumulator->reset();


	commands = { "a","{","b","{","c","}", "e", "}" };
	expected_state = 1;
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_state(), expected_state);
	test_accumulator->reset();

}

TEST(Accumulator_test, check_add_handlers) {
	//ѕровер€ем правильность работы функции добавлени€ Handlers

	int bulk = 5;
	auto test_accumulator = std::shared_ptr<Accumulator>(new Accumulator(bulk));

	ASSERT_EQ(test_accumulator->get_handlers().size(), 0);

	std::vector<std::shared_ptr<Test_handler>> handler_vec;
	std::shared_ptr<Test_handler> handler;
	for (size_t i = 0; i < 5; i++)
	{
		handler = std::shared_ptr<Test_handler>(new Test_handler);
		handler_vec.push_back(handler);
		test_accumulator->add_handler(handler);
		for (size_t j = 0; j <= i; j++)
		{
			EXPECT_EQ(test_accumulator->get_handlers()[j], handler_vec[j]);
		}
		ASSERT_EQ(test_accumulator->get_handlers().size(), i+1);
	}

}

TEST(Accumulator_test, check_block_size_with_static_commands) {
	//ѕроверка на правильность размера буфера команд (статический размер)
	int bulk = 5;
	auto test_accumulator = std::shared_ptr<Accumulator>(new Accumulator(bulk));

	ASSERT_EQ(test_accumulator->get_command_block().size(), 0);

	std::vector<std::string> commands{ "a" };
	std::vector<std::string> expected_vector;

	expected_vector = commands;
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("b");
	expected_vector = commands;
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("c");
	expected_vector = commands;
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("d");
	expected_vector = commands;
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("e");
	expected_vector = {};
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), 0);
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("f");
	expected_vector = { "f" };
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size()% bulk);
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	commands.push_back("g");
	expected_vector = { "f" ,"g"};
	test_accumulator->reset();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), commands.size() % bulk);
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
}

TEST(Accumulator_test, check_block_size_with_dynamic_commands) {
	//ѕроверка на правильность размера буфера команд (динамический размер)

	int bulk = 2;
	auto test_accumulator = std::shared_ptr<Accumulator>(new Accumulator(bulk));

	ASSERT_EQ(test_accumulator->get_command_block().size(), 0);

	std::vector<std::string> commands{ "a","{","b","c","d","e","f" };
	std::vector<std::string> expected_vector{ "b","c","d","e","f" };
	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());

	test_accumulator->reset();

	commands = { "a","{","b", "{","c","d","e","f" };
	expected_vector = { "b","c","d","e","f" };
//	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
	test_accumulator->reset();


	commands = { "a","{","b", "{","c","d","e","f","{","g" };
	expected_vector = { "b","c","d","e","f","g" };
	//	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
	test_accumulator->reset();

	commands = { "a","{","b", "{","c","d","e","f","{","g","}" };
	expected_vector = { "b","c","d","e","f","g" };
	//	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
	test_accumulator->reset();

	commands = { "a","{","b", "{","c","d","e","f","{","g","}", "h","}" };
	expected_vector = { "b","c","d","e","f","g","h" };
	//	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
	test_accumulator->reset();

	commands = { "a","{","b", "{","c","d","e","f","{","g","}", "h","}","}" };
	expected_vector = {};
	//	test_accumulator->clear_block();
	accumulator_adder(test_accumulator, commands);
	ASSERT_EQ(test_accumulator->get_command_block().size(), expected_vector.size());
	EXPECT_EQ(expected_vector, test_accumulator->get_command_block());
	test_accumulator->reset();

}

