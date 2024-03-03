#include "server.h"
#include "session.h"
#include "async.h"


Server::Server(boost::asio::io_context& io_context, short port, std::size_t bulk)
	:bulk_(bulk), acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
	start_accept();
}

/*Server::Server(boost::asio::io_context& io_context, short port, std::size_t bulk)
	: bulk_(bulk), acceptor_(io_context, tcp::endpoint(boost::asio::ip::address::from_string("::1"), port))
{
	start_accept();
}*/




void Server::start_accept() {
	acceptor_.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<Session>(std::move(socket),async::connect(bulk_))->start();
			}
			start_accept();
		}

	);
};