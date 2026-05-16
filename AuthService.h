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
		int userId;
		std::string accessToken;
		std::string refreshToken;
		std::string errorMessage;
	};

	struct TokenValidationResult {
		bool isValid = false;
		std::string userId;
		std::string accessToken;
		std::string refreshToken;
		std::string errorMessage;
	};


	LoginResult login(std::string login,std::string password);
	bool registerUser(std::string user,std::string email, std::string password);
	TokenValidationResult validateToken(const std::string& accessToken,const std::string& refreshToken);

	std::string extractUserIdFromToken(const std::string& accessToken);
	std::string jwtSecret_ = "JWT_SECRET_KEY";
private:
	std::shared_ptr<UserRepository> userRepository_;





};

#endif // !AUTHSERVICE_H

