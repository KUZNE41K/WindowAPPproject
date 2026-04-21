#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "UserRepository.h"
#include "Request.h"

class UserRepository;

class AuthService
{
public:
	AuthService(std::shared_ptr<UserRepository> repo);

	struct LoginResult
	{
		bool success;
		std::string accessToken;
		std::string refreshToken;
		std::string errorMessage;
	};



	LoginResult login(std::string login,std::string password);
	bool registerUser(std::string user,std::string email, std::string password);
private:
	std::shared_ptr<UserRepository> userRepository_;





};

#endif // !AUTHSERVICE_H

