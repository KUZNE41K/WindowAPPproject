#ifndef CREATESESSIONHANDLER_H
#define CREATESESSIONHANDLER_H
#include "Handler.h"
#include "Request.h"
#include "UserRepository.h"
#include <string>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <memory>

class CreateSessionHandler : public Handler
{
public:
	explicit CreateSessionHandler(std::shared_ptr<UserRepository> repo);

	void handle(Request& request) override;

private:
	std::string jwtSecret_;
	std::shared_ptr<UserRepository> repo_;
	std::string generateJwtToken(const std::string& userId);
	bool saveSessionToDatabase(const std::string& userId, const std::string& token, int lifetimeSeconds);
};


#endif // !CREATESESSIONHANDLER_H
