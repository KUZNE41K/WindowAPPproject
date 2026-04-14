#include "Router.h"
#include "RequestParser.h"

Router::Router(std::shared_ptr<AuthService> authService) : authService_(authService)
{
}

std::shared_ptr<Request> Router::route(const std::string& path, std::string& body)
{
	auto request = RequestParser::parseRequest(body);

	if (path == "/login")
	{
		authService_->login(request->login_, request->password_);
	}
	else if (path == "/register")
	{
		authService_->registerUser(request->login_, request->email_ , request->password_);
	}
	else
	{
		request->setSuccess(false);
		request->setErrorMessage("Unknown path: " + path);
	}
	return request;
}
