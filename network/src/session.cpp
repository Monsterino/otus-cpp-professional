#include "session.h"


Session::Session(tcp::socket socket, Context* context)
	:socket_(std::move(socket)),context_(context)
{
}


void Session::start() {
	do_read();
}




void Session::do_read() {
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(buffer, BUFFER_SIZE),
		[this,self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				async::receive(context_, buffer, length);
				do_read();
				
			}
			else if ((ec == boost::asio::error::eof) || (ec == boost::asio::error::connection_reset))
			{
				async::disconnect(context_);
			}
	
		});
}
