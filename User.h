#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
	std::string login_;
	std::string email_;
	std::string password_;
	std::string salt_;

	User(const std::string& login,
		const std::string& email,
		const std::string& password,
		const std::string& salt) : login_(login), email_(email), password_(password), salt_(salt) {}
};

#endif // USER_H