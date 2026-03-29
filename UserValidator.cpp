#include "UserValidator.h"

bool UserValidator::identification(const User& user, const std::string& login)
{
	if (user.login_ != login)
	{
		std::cout << "Login does not match." << std::endl;
		return false;
	}
}
/*
bool UserValidator::registration(const User& user, const std::string& login, const std::string& email, const std::string& password)
{
	if (user.login_ != login && user.password_ != password)
	{
		std::cout << "Registration is failed";
		return false;
	}
}*/
