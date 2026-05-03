#ifndef CREATESESSIONHANDLER_H
#define CREATESESSIONHANDLER_H
#include "Handler.h"
#include "Request.h"
#include "Hash.h"
#include "UserRepository.h"
#include <string>
#include <stduuid/uuid.h>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <memory>

class CreateSessionHandler : public Handler
{
public:
	explicit CreateSessionHandler(std::shared_ptr<UserRepository> repo);
	explicit CreateSessionHandler();

	void handle(Request& request) override;

	static std::string generate();
	std::string generateJwtToken(const std::string& userId);

private:

	std::string jwtSecret_ = "JWT_SECRET_KEY";
	std::shared_ptr<UserRepository> repo_;
	
	bool saveSessionToDatabase(const std::string& userId, const std::string& refreshToken, const std::string& parentTokenHash, int lifetimeSeconds);
};


#endif // !CREATESESSIONHANDLER_H
