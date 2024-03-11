#include <gtest/gtest.h>

#include "error_codes.h"

#include <string>



TEST(Message_test, check_error) {
	//auto test_table = std::shared_ptr<Table>()

	auto test_ok_code = Message(Message::Code::OK, "OK");
	auto test_error_code = Message(Message::Code::ERR, "Test Err");

	ASSERT_EQ(test_ok_code.get_code(), Message::Code::OK);
	ASSERT_EQ(test_error_code.get_code(), Message::Code::ERR);

	ASSERT_EQ(test_ok_code.get_message(),"OK");
	ASSERT_EQ(test_error_code.get_message(), "Test Err");

	std::string test_string_ok = Message::code_to_string(test_ok_code);
	std::string test_string_error = Message::code_to_string(test_error_code);

	std::string expected_ok = "OK\n";
	std::string expected_error = "ERR Test Err\n";

	ASSERT_EQ(test_string_ok, expected_ok);
	ASSERT_EQ(test_string_error, expected_error);
}
