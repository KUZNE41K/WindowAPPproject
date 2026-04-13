#include "Session.h"
#include "RequestParser.h"
#include "ResponseBuilder.h"

Session::Session(tcp::socket socket, std::shared_ptr<Router> router) : socket_(std::move(socket)), router_(router)
{
}
void Session::start()
{
	doRead();
}

void Session::doRead()
{
	auto self =shared_from_this();
	
	async_read(socket_, buffer_, request_,
		[self](beast::error_code ec, std::size_t bytes_transferred)
		{
			if (ec)
			{
				return;
			}
			self->handleRequest();
		});
}

void Session::handleRequest()
{
	std::string path = request_.target();
	std::string body = request_.body();

	auto resultRequest = router_->route(path, body);

	std::string responseBuilder = ResponseBuilder::buildResponse(*resultRequest);

	response_.version(request_.version());
	response_.set(http::field::content_type,"application/json");

	if (resultRequest->success_)
	{
		response_.result(http::status::ok);
	}
	else
	{
		response_.result(http::status::bad_request);
	}

	response_.body() = responseBuilder;
	response_.keep_alive(request_.keep_alive());
	response_.prepare_payload();
	doWrite();

}
void Session::doWrite()
{
	auto self = shared_from_this();

	http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t)
		{
			if (ec)
			{
				return;
			}
			if (self->response_.keep_alive())
			{
				self->request_ = {};
				self->response_ = {};
				self->buffer_.consume(self->buffer_.size());
				self->doRead();
			}
			else
			{
				beast::error_code ec_close;
				self->socket_.shutdown(tcp::socket::shutdown_send, ec_close);
			}
		});
}