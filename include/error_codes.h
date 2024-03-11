#pragma once
#include <string>


class Message
{
public:
	enum class Code {
		OK,
		ERR,
	};


	Message(Code code, std::string message);
	Message() = delete;
	Code get_code();
	std::string get_message();

	static std::string code_to_string(Message message);


private:
	Code code_;
	std::string message_;
};