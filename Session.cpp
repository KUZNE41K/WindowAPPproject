#include "Session.h"
#include "RequestParser.h"
#include "ResponseBuilder.h"
#include <iostream>

Session::Session(tcp::socket socket, std::shared_ptr<Router> router)
    : socket_(std::move(socket)), router_(router)
{
    std::cout << "Session created" << std::endl;
}

void Session::start()
{
    std::cout << "Session started" << std::endl;
    doRead();
}

void Session::doRead()
{
    auto self = shared_from_this();

    async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred)
        {
            if (ec)
            {
                std::cerr << "Read error: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Received " << bytes_transferred << " bytes" << std::endl;
            self->handleRequest();
        });
}

void Session::handleRequest()
{
    std::cout << "=== Session::handleRequest ===" << std::endl;

    std::string path = request_.target();
    std::string body = request_.body();

    std::cout << "Path: " << path << std::endl;
    std::cout << "Body: " << body << std::endl;

    auto resultRequest = router_->route(path, body);

    std::cout << "After route, success: " << resultRequest->success_ << std::endl;
    std::cout << "Response body: " << resultRequest->responseBody_ << std::endl;

    response_.version(request_.version());
    response_.set(http::field::content_type, "application/json");
    response_.set(http::field::content_length, std::to_string(resultRequest->responseBody_.size()));

    if (resultRequest->success_)
    {
        response_.result(http::status::ok);
    }
    else
    {
        response_.result(http::status::bad_request);
    }

    response_.body() = resultRequest->responseBody_;
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
                std::cerr << "Write error: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Response sent" << std::endl;

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