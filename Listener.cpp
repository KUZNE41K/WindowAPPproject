#include "Listener.h"
#include "WebSocketSession.h"
#include "SharedState.h"



Listener::Listener(
	net::io_context& ioc,
	const std::string& host,
	unsigned short port,
	std::shared_ptr<SharedState> sharedState,
	const std::string& docRoot
) : ioc_(ioc), acceptor_(ioc), sharedState_(std::move(sharedState)), docRoot_(std::move(docRoot)),
	host_(host), port_(port)
{
}

void Listener::run()
{
	beast::error_code ec;
	tcp::endpoint endpoint{ net::ip::make_address(host_), port_ };

	if (ec)
	{
		std::cerr << "Failed to parse address: " << ec.message() << std::endl;
		return;
	}

	acceptor_.open(endpoint.protocol(), ec);
	if (ec)
	{
		std::cerr << "[Listener]Failed to open acceptor: " << ec.message() << std::endl;
		return;
	}
	acceptor_.set_option(net::socket_base::reuse_address(true), ec);

	acceptor_.bind(endpoint, ec);
	if (ec)
	{
		std::cerr << "[Listener]Failed to bind acceptor: " << ec.message() << std::endl;
		return;
	}
	acceptor_.listen(net::socket_base::max_listen_connections, ec);
	if (ec)
	{
		std::cerr << "[Listener]Failed to listen on acceptor: " << ec.message() << std::endl;
		return;
	}
	std::cout << "[Listener] Listening on " << host_ << ":" << port_ << std::endl;
	doAccept();
}

void Listener::stop()
{
	beast::error_code ec;
	acceptor_.close(ec);
	if (ec)
	{
		std::cerr << "[Listener] Failed to close acceptor: " << ec.message() << std::endl;
	}
	else
	{
		std::cout << "[Listener] Acceptor closed successfully." << std::endl;
	}
}

void Listener::onAccept(beast::error_code ec, tcp::socket socket)
{
	if (ec)
	{
		std::cerr << "[Listener] Accept error: " << ec.message() << std::endl;
	}
	else
	{
		std::cout << "[Listener] New connection accepted" << std::endl;
		std::make_shared<WebSocketSession>(std::move(socket), sharedState_, docRoot_)->run();
	}
	doAccept();
}

void Listener::doAccept()
{
	acceptor_.async_accept(
		net::make_strand(ioc_),
		beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
}