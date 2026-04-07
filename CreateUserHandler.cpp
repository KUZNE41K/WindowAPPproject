#include "CreateUserHandler.h"

CreateUserHandler::CreateUserHandler(UserRepository& repo) : repo_(repo){}


void CreateUserHandler::handle(Request& request)
{
	if (request.user_ != nullptr)
	{
		request.setSuccess(false);
		request.setErrorMessage("User already exists.");
		return;
	}
	createUser(request);

	if (next_)
	{
		next_->handle(request);
	}
	
}

void CreateUserHandler::createUser(Request& request)
{
	std::string passwordHash = Hash::hash(request.password_);
	auto userPtr = std::make_shared<User>();
	userPtr->setLogin(request.login_);
	userPtr->setEmail(request.email_);
	userPtr->setPasswordHash(passwordHash);

	if (!repo_.saveUser(userPtr))
	{
		request.setSuccess(false);
		request.setErrorMessage("Failed to create user.");
		return;
	}

	request.user_ = userPtr;
	request.setSuccess(true);
	request.setErrorMessage("");
	
}
