#include "session.h"


Session::Session(tcp::socket socket)
	:socket_(std::move(socket)),parser_(std::unique_ptr<Parser>(new Parser('\n'))){}


void Session::start() {
	do_read();
}

void Session::do_read() {
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(buffer, BUFFER_SIZE),
		[this,self](boost::system::error_code ec, std::size_t length)
		{
			std::string return_message;
			if (!ec)
			{
				parser_->prepare_commands(std::string{ buffer, length});
				while (parser_->has_commands())
				{
					return_message = parser_->execute_command();
					do_write(return_message);
				}
				
			}

		do_read();
		});
}

void Session::do_write(const std::string& message)
{
	auto self(shared_from_this());
	
	socket_.async_send(boost::asio::buffer(message.c_str(), message.size()), [this,self](boost::system::error_code ec, std::size_t length){});
}


