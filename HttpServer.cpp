#include "HttpServer.h"
#include "Session.h"

HttpServer::HttpServer(net::io_context& io_context, tcp::endpoint endpoint, std::shared_ptr<Router> router) :acceptor_(net::make_strand(io_context)),
socket_(net::make_strand(io_context)), router_(router)
{
	beast::error_code ec;

	acceptor_.open(endpoint.protocol(), ec);
	if (ec)
	{
		std::cerr << "Failed to open acceptor: " << ec.message() << std::endl;
		return;
	}
	
	acceptor_.set_option(net::socket_base::reuse_address(true), ec);
	if (ec)
	{
		std::cerr << "Failed to set reuse_address option: " << ec.message() << std::endl;
		return;
	}
	
	acceptor_.bind(endpoint, ec);
	if (ec)
	{
		std::cerr << "Failed to bind acceptor: " << ec.message() << std::endl;
		return;
	}

	acceptor_.listen(net::socket_base::max_listen_connections, ec);
	if(ec)
	{
		std::cerr << "Failed to listen on acceptor: " << ec.message() << std::endl;
		return;
	}
}







void HttpServer::run()
{
	doAccept();
}


void HttpServer::doAccept()
{
	acceptor_.async_accept(
		net::make_strand(acceptor_.get_executor()),
		[this](beast::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<Session>(std::move(socket), router_)->start();
			}
			else
			{
				std::cerr << "Failed to accept connection: " << ec.message() << std::endl;
			}
			doAccept();
		});
}
