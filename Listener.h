#ifndef LISTENER_H
#define LISTENER_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <string>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class SharedState;
class WebSocketSession;

class Listener : public std::enable_shared_from_this<Listener>
{
public:
    Listener(
        net::io_context& ioc,
		const std::string& host,
		unsigned short port,
		std::shared_ptr<SharedState> sharedState,
		const std::string& docRoot
    );

	void run();
	void stop();
private:
    void onAccept(beast::error_code ec, tcp::socket socket);
    void doAccept();

	net::io_context& ioc_;
	tcp::acceptor acceptor_;
	std::shared_ptr<SharedState> sharedState_;
	std::string docRoot_;
	std::string host_;
	unsigned short port_;

};


#endif // !LISTENER_H
