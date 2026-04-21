#include "UserFetchHandler.h"

UserFetchHandler::UserFetchHandler(std::shared_ptr<UserRepository> repo): userRepository_(repo)
{
}

void UserFetchHandler::handle(Request& request)
{
	if(!userRepository_)
	{
		request.setErrorMessage("User repository is not set.");
		request.success_ = false;
		return;
	}
	std::shared_ptr<User> user = nullptr;

	if(!request.login_.empty())
	{
		user = userRepository_->findUserByLogin(request.login_);
	}
	else if(!request.email_.empty())
	{
		user = userRepository_->findUserByEmail(request.email_);
	}
	else
	{
		request.setErrorMessage("No login or email provided.");
		request.success_ = false;
		return;
	}
	request.setUser(user);
	request.success_ = true;
	request.setErrorMessage("");

	if (next_)
	{
		next_->handle(request);
	}


}

