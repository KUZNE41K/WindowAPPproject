#include "Request.h"

void Request::setSuccess(bool success)
{
	success_ = false;
	errorMessage_ = "";
}

void Request::setErrorMessage(const std::string& errorMessage)
{
	success_ = false;
	errorMessage_ = errorMessage;
}

void Request::setUser(std::shared_ptr<User> user)
{
	user_ = user;
}

