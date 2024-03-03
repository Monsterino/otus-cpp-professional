#pragma once 
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_context& io_context, short port, std::size_t bulk);
	~Server() = default;
private:
	void start_accept();

	std::size_t bulk_;
	tcp::acceptor acceptor_;

};

