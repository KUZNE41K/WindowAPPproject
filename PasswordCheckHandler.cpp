#include "PasswordCheckHandler.h"

PasswordCheckHandler::PasswordCheckHandler()
{
}

void PasswordCheckHandler::handle(Request& request)
{
	if(request.user_ == nullptr)
	{
		request.setSuccess(false);
		request.setErrorMessage("User does not exist.");
		return;
	}

	const std::string& password = request.password_;
	const std::string& hash = request.user_->getPasswordHash();

	if (!checkPassword(password, hash))
	{
		request.setSuccess(false);
		request.setErrorMessage("Invalid password.");
		return;
	}
	if (next_)
	{
		next_->handle(request);
	}
	
}

bool PasswordCheckHandler::checkPassword(const std::string& password, const std::string& hash)
{
	return Hash::verifyPassword(password,hash);
}

