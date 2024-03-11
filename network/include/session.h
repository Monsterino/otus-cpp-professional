#pragma once 

#include <boost/asio.hpp>
#include "command_parser.h"


using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket) ;
	~Session() = default;
	void start();
	

private:
	void do_read();
	void do_write(const std::string& message);
	static constexpr std::size_t BUFFER_SIZE = 1024;
	tcp::socket socket_;
	char buffer[BUFFER_SIZE];
	std::unique_ptr<Parser> parser_;


};

