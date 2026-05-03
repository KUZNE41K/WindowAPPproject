#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <optional>
#include <chrono>
#include <ctime>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;        // from <nlohmann/json.hpp>

class SharedState;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
	explicit WebSocketSession(tcp::socket&& socket, std::shared_ptr<SharedState> sharedState, std::string docRoot);

	~WebSocketSession();

	void run();
	void sendMessage(const std::string& message);

	void sendMessage(const json& message);
	void doClose(websocket::close_reason const& reason);

	void setUserId(const std::string& userId);
	std::string getUserId() const;
	std::string getSessionId() const;

private:
	void onAccept(beast::error_code ec);
	void doRead();
	void onRead(beast::error_code ec, std::size_t bytesTransferred);
	void onMessage(json const& message);
	void doWrite();
	void onWrite(beast::error_code ec, std::size_t bytesTransferred);
	void onClose(beast::error_code ec);
	void onReadHttp(beast::error_code ec, std::size_t bytesTransferred);

	bool authenticate(const http::request<http::string_body>& req);

	std::optional<json> parseJson(const std::string& text);

	void handleChatMessage(json const& payload);
	void handlePrivateMessage(json const& payload);
	void handleTypingNotification(json const& payload);

	json makeResponse(std::string const& type, json const& payload);
	json makeErrorResponse(std::string const& error_message, int code = 400);
	std::string generateSessionId();

	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;
	http::request_parser<http::string_body> parser_;
	std::shared_ptr<SharedState> sharedState_;
	std::string sessionId_;
	std::string userId_;
	std::string docRoot_;

	std::queue<std::shared_ptr<std::string const>> writeQueue_;
	bool writeInProgress_ = false;
	bool closeSent_ = false;
	std::mutex writeMutex_;
	
	static constexpr std::size_t MAX_MESSAGE_SIZE = 16 * 1024 * 1024;
};


#endif // !WEBSOCKETSESSION_H

