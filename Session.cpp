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
    buffer_.clear();
    request_ = {};

    async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                std::cout << "Received " << bytes_transferred << " bytes" << std::endl;
                self->handleRequest();
            }
            if (ec == http::error::end_of_stream)
            {
                std::cout << "Client closed connection" << std::endl;
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            }
            else
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
    if (request_.target() == "/") {
        response_.version(11);
        response_.result(http::status::ok);
        response_.set(http::field::content_type, "text/plain");
        response_.set(http::field::connection, "close");
        response_.body() = "OK";
        response_.prepare_payload();
        doWrite();
        return;
    }

    std::cout << "=== Session::handleRequest ===" << std::endl;

    std::string path = request_.target();
    std::string body = request_.body();
    std::string method = std::string(request_.method_string());

    std::cout << "Path: " << path << std::endl;
    std::cout << "Body: " << body << std::endl;

    auto resultRequest = router_->route(path, body,method);

    std::cout << "After route, success: " << resultRequest->success_ << std::endl;
    std::cout << "Response body: " << resultRequest->responseBody_ << std::endl;

    http::response<http::string_body> response;
    response_.version(request_.version());
    response_.set(http::field::content_type, "application/json");
    response_.set(http::field::content_length, std::to_string(resultRequest->responseBody_.size()));
    response_.set(http::field::connection, "close");

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
                beast::error_code ignored;
                self->socket_.shutdown(tcp::socket::shutdown_send, ignored);
                self->socket_.close(ignored);
                return;
            }
            std::cout << "Response sent" << std::endl;

            if (self->response_.keep_alive())
            {
                self->buffer_.clear();
                self->request_ = {};
                self->response_ = {};
                self->doRead();
            }
            else
            {
                beast::error_code ec_close;
                self->socket_.shutdown(tcp::socket::shutdown_send, ec_close);
                self->socket_.close(ec_close);
            }
        });
}