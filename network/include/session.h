#pragma once 

#include <boost/asio.hpp>
#include "async.h"
#include "context.h"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, Context* context) ;
	~Session() = default;
	void start();
	

private:
	void do_read();
	static constexpr std::size_t BUFFER_SIZE = 1024;
	tcp::socket socket_;
	char buffer[BUFFER_SIZE];

	Context* context_;

};

