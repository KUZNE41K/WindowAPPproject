#include "WebSocketSession.h"
#include "SharedState.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <jwt-cpp/jwt.h>

void fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}

WebSocketSession::WebSocketSession(tcp::socket&& socket, std::shared_ptr<SharedState> sharedState, std::string docRoot) : ws_(std::move(socket)), sharedState_(std::move(sharedState)),
docRoot_(std::move(docRoot)), sessionId_(generateSessionId())
{
	ws_.binary(false);
	ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));
	ws_.set_option(websocket::stream_base::decorator(
		[](websocket::response_type& res)
		{
			res.set(beast::http::field::server,"websocket-server-async");
		}));
	ws_.read_message_max(MAX_MESSAGE_SIZE);
}

WebSocketSession::~WebSocketSession()
{
}
void WebSocketSession::run()
{
	http::async_read(ws_.next_layer(),
		buffer_,
		parser_,
		beast::bind_front_handler(&WebSocketSession::onReadHttp,shared_from_this())
	);
}

void WebSocketSession::sendMessage(const std::string& message)
{
	auto msg = std::make_shared<std::string const>(message);
	std::lock_guard<std::mutex> lock(writeMutex_);

	if (closeSent_)
	{
		return;
	}

	writeQueue_.push(msg);
	if (!writeInProgress_)
	{
		writeInProgress_ = true;
		doWrite();
	}
}

void WebSocketSession::sendMessage(const json& message)
{
	sendMessage(message.dump());
}

void WebSocketSession::doClose(websocket::close_reason const& reason)
{
	if (closeSent_)
	{
		return;
	}

	closeSent_ = true;

	beast::error_code ec;
	ws_.close(reason, ec);

}

void WebSocketSession::setUserId(const std::string& userId)
{
	userId_ = std::move(userId);
}

std::string WebSocketSession::getUserId() const
{
	return userId_;
}

std::string WebSocketSession::getSessionId() const
{
	return sessionId_;
}

void WebSocketSession::onAccept(beast::error_code ec)
{
	if (ec)
	{
		return fail(ec,"accept");
	}
	buffer_.consume(buffer_.size());
	sharedState_->join(sessionId_, shared_from_this());
	if (!userId_.empty())
	{
		sharedState_->bindUser(sessionId_, userId_);
	}
	doRead();
}

void WebSocketSession::doRead()
{
	ws_.async_read(buffer_,
		beast::bind_front_handler(&WebSocketSession::onRead,
			shared_from_this()));
}

void WebSocketSession::onRead(beast::error_code ec, std::size_t bytesTransferred)
{
	boost::ignore_unused(bytesTransferred);

	if (ec == websocket::error::closed)
	{
		return;
	}

	if(ec)
			{
		return fail(ec, "read");
	}

	std::string rawMessage = beast::buffers_to_string(buffer_.data());
	buffer_.consume(buffer_.size());

	auto jsonMessage = parseJson(rawMessage);
	if (jsonMessage)
	{
		onMessage(*jsonMessage);
	}
	else
	{
		sendMessage(makeErrorResponse("Invalid JSON", 400));
	}

	if (!closeSent_)
	{
		doRead();
	}
}

void WebSocketSession::onMessage(json const& message)
{

	if (!message.contains("type"))
	{
		sendMessage(makeErrorResponse("Message type required", 400));
		return;
	}
	std::string type = message["type"].get<std::string>();
	json payload = message.value("payload", json::object());
	if (type == "chat_message")
	{
		handleChatMessage(payload);
	}
	else if (type == "private_message")
	{
		handlePrivateMessage(payload);
	}
	else if (type == "typing")
	{
		handleTypingNotification(payload);
	}
	else
	{
		sendMessage(makeErrorResponse("Unknown message type: " + type, 400));
	}
}


void WebSocketSession::doWrite()
{
	if (writeQueue_.empty())
	{
		writeInProgress_ = false;
		return;
	}

	auto message = writeQueue_.front();

	ws_.async_write(
		net::buffer(*message),
		beast::bind_front_handler(&WebSocketSession::onWrite,shared_from_this()));
}

void WebSocketSession::onWrite(beast::error_code ec, std::size_t bytesTransferred)
{
	boost::ignore_unused(bytesTransferred);

	if (ec)
	{
		fail(ec,"write");

		sharedState_->leave(sessionId_);
		return;
	}

	{
		std::lock_guard<std::mutex> lock(writeMutex_);

		writeQueue_.pop();
	}
	doWrite();
}

void WebSocketSession::onClose(beast::error_code ec)
{
	if (ec)
	{
		return fail(ec, "close");
	}
	sharedState_->leave(sessionId_);
}

void WebSocketSession::onReadHttp(beast::error_code ec, std::size_t bytesTransferred)
{
	boost::ignore_unused(bytesTransferred);
	if (ec)
	{
		if (ec == http::error::bad_method ||
			ec == http::error::bad_version ||
			ec == net::error::eof)
		{
			std::cerr << "[WebSocket] Invalid HTTP request from client" << std::endl;
		}
		else if (ec != net::error::operation_aborted)
		{
			return fail(ec, "http_read");
		}
		beast::error_code close_ec;
		ws_.next_layer().close();
		sharedState_->leave(sessionId_);
		return;
		
	}
	if (!websocket::is_upgrade(parser_.get()))
	{
		http::response<http::string_body> res{
			http::status::bad_request,
			parser_.get().version(), };
		res.set(http::field::server, "WebSocket-Chat-Server");
		res.set(http::field::content_type,"text/plain");
		res.body() = "Expected WebSocket Upgrade request";
		res.prepare_payload();

		beast::http::write(ws_.next_layer(), res);
		return;
	}

	if (!authenticate(parser_.get()))
	{
		http::response<http::string_body> res{
			http::status::unauthorized,
			parser_.get().version(), };
		res.set(http::field::server, "WebSocket-Chat-Server");
		res.set(http::field::content_type, "text/plain");
		res.body() = "Authorization token required";
		res.prepare_payload();

		beast::http::write(ws_.next_layer(), res);
		return;
	}

	ws_.async_accept(parser_.get(),
		beast::bind_front_handler(&WebSocketSession::onAccept,shared_from_this()));

}

bool WebSocketSession::authenticate(const http::request<http::string_body>& req)
{
	auto authIt = req.find(http::field::authorization);
	if (authIt == req.end())
	{
		return false;
	}
	std::string authtoken = std::string(authIt->value());

	if (!authtoken.starts_with("Bearer "))
	{
		return false;	

	}
	std::string token = authtoken.substr(7);

	if (token.empty())
	{
		return false;
	}

	try
	{
		auto decoded = jwt::decode(token);

		auto verifier = jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256{sharedState_->getJwtKey()}) // временно далее выгружать из sharedState а там из конфига
			.with_issuer("MyApp");
		verifier.verify(decoded);

		std::string userId = decoded.get_subject();
		if (userId.empty())
		{
			return false;
		}
		setUserId(userId);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}

}

std::optional<json> WebSocketSession::parseJson(const std::string& text)
{
	try
	{
		return json::parse(text);
	}
	catch (const json::parse_error&)
	{
		return std::nullopt;
	}
	
}

void WebSocketSession::handleChatMessage(json const& payload)
{
	if (userId_.empty())
	{
		sendMessage(makeErrorResponse("Authenticate first", 401));
		return;
	}
	if (!payload.contains("text"))
	{
		sendMessage(makeErrorResponse("Message text required", 400));
		return;
	}

	json broadcastMessage = makeResponse("chat_message", {
		{"from", userId_},
		{"text", payload["text"].get<std::string>()},
		{"sessionId", sessionId_}
		});
	sharedState_->broadcast(broadcastMessage.dump());

}

void WebSocketSession::handlePrivateMessage(json const& payload)
{
	if (userId_.empty())
	{
		sendMessage(makeErrorResponse("Authenticate first", 401));
		return;
	}
	if (!payload.contains("to") || !payload.contains("text"))
	{
		sendMessage(makeErrorResponse("Recipient and message text required", 400));
		return;
	}
	std::string toUser = payload["to"].get<std::string>();

	json privateMessage = makeResponse("private_message", {
		{"from", userId_},
		{"text", payload["text"].get<std::string>()},
		{"sessionId", sessionId_}
		});

	bool sent = sharedState_->sendMessageToUser(toUser, privateMessage.dump());

	if (sent)
	{
		json confirmation = makeResponse("private_message_sent", {
			{"to", toUser},
			{"status", "delivered"}
		});
		sendMessage(confirmation);
	}
	else
	{
		sendMessage(makeErrorResponse("User " + toUser + " is offline", 404));
	}

}

void WebSocketSession::handleTypingNotification(json const& payload)
{
	if (userId_.empty())
	{
		sendMessage(makeErrorResponse("Authenticate first", 401));
		return;
	}
	
	json typingNotification = makeResponse("typing_notification", {
		{"userId", userId_},
		{"isTyping", payload.value("isTyping", true)}
		});

	sharedState_->broadcast(typingNotification.dump());
}

json WebSocketSession::makeResponse(std::string const& type, json const& payload)
{
	return json{
		{"type", type},
		{"payload", payload},
		{"timestamp", std::time(nullptr)}
	};
}

json WebSocketSession::makeErrorResponse(std::string const& error_message, int code)
{
	return json{
		{"type", "error"},
		{"code", code},
		{"message",error_message},
		{"timestamp", std::time(nullptr)}
	};
}

std::string WebSocketSession::generateSessionId()
{
	static boost::uuids::random_generator generator;
	return boost::uuids::to_string(generator());
}
