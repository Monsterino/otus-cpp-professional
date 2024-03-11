#include "error_codes.h"


Message::Message(Code code, std::string message) :code_(code), message_(message) {};

Message::Code Message::get_code() {
	return code_;
}
std::string Message::get_message() {
	return message_;
}


std::string Message::code_to_string(Message message) {
	switch (message.get_code())
	{
	case Message::Code::OK:
		return ("OK\n");
	case Message::Code::ERR:
		return ("ERR " + message.get_message()+"\n");
	default:
		return ("ERR " + message.get_message() + "\n");
	}
}