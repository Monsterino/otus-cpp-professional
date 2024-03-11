#include <gtest/gtest.h>

#include "error_codes.h"
#include "table.h"

#include <string>


void add_data_first(std::shared_ptr<Table> table) {
	table->insert(0, "lean");
	table->insert(1, "sweater");
	table->insert(2, "frank");
	table->insert(3, "violation");
	table->insert(4, "quality");
	table->insert(5, "precision");
}

void add_data_second(std::shared_ptr<Table> table) {
	table->insert(3, "proposal");
	table->insert(4, "example");
	table->insert(5, "lake");
	table->insert(6, "flour");
	table->insert(7, "wonder");
	table->insert(8, "selection");
}

TEST(Table_test, insertion_check) {
	auto test_table = std::shared_ptr<Table>(new Table);

	ASSERT_EQ(test_table->insert(0, "lean"), "OK\n");
	ASSERT_EQ(test_table->insert(0, "understand"), "ERR duplicate 0\n");

	ASSERT_EQ(test_table->insert(1, "sweater"), "OK\n");
	ASSERT_EQ(test_table->insert(2, "frank"), "OK\n");
	ASSERT_EQ(test_table->insert(3, "violation"), "OK\n");
	ASSERT_EQ(test_table->insert(4, "quality"), "OK\n");
	ASSERT_EQ(test_table->insert(5, "precission"), "OK\n");

}

TEST(Table_test, check_data) {
	auto test_table = std::shared_ptr<Table>(new Table);

	std::map<int, std::string> expected_data = {
		{0,"lean"},
		{1,"sweater"},
		{2,"frank"},
		{3,"violation"},
		{4,"quality"},
		{5,"precision"}	
	};

	add_data_first(test_table);


	ASSERT_EQ(expected_data,test_table->get_data() );

}

TEST(Table_test, check_intersection) {
	auto test_table_A = std::shared_ptr<Table>(new Table);
	auto test_table_B = std::shared_ptr<Table>(new Table);

	std::string expected_data("3,violation,proposal\n4,quality,example\n5,precision,lake\nOK\n");

	add_data_first(test_table_A);
	add_data_second(test_table_B);

	ASSERT_EQ(expected_data, test_table_A->intersection(*test_table_B));
}

TEST(Table_test, check_symmetric_difference) {
	auto test_table_A = std::shared_ptr<Table>(new Table);
	auto test_table_B = std::shared_ptr<Table>(new Table);

	std::string expected_data("0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection\nOK\n");

	add_data_first(test_table_A);
	add_data_second(test_table_B);

	//	std::cout << test_table_A->intersection(*test_table_B) << std::endl;
	ASSERT_EQ(expected_data, test_table_A->symmetric_difference(*test_table_B));

}

TEST(Table_test, check_truncate) {
	auto test_table_A = std::shared_ptr<Table>(new Table);
	
	std::map<int, std::string> expected_data{};

	add_data_first(test_table_A);
	test_table_A->clear();
	ASSERT_EQ(expected_data, test_table_A->get_data());

}