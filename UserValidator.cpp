#include "UserValidator.h"

bool UserValidator::authenticate(const User& user, const std::string& login, const std::string& password)
{
	if (user.login_ != login)
	{
		return false;
	}
}
